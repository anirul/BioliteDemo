/*
 * Copyright (c) 2006-2010, anirul
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Calodox nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY anirul ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL anirul BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "xml_misc.h"
#include "xml_image.h"
#include "media_collection.h"

xml_image* xml_image::parseImage(
	irr::io::IXMLReaderUTF8* xml,
	irr::IrrlichtDevice* pdevice) 
{
	bool is_position = false;
	bool is_size = false;
	bool is_location = false;
	irr::core::vector2df position(0.0f, 0.0f);
	irr::core::vector2df size(0.0f, 0.0f);
	std::string location("");
	xml_misc::coordinate_type ct = xml_misc::absolute;
	if (xml->getAttributeCount() > 1)
		throw std::runtime_error(
			"XML PARSE ERROR : xml attribute of image > 1");
	if (xml->getAttributeCount() > 0) {
		if (std::string(xml->getAttributeName(0)) == std::string("coordinate")) 
		{
			std::string coordinate = xml->getAttributeValue(0);
			if (coordinate == std::string("absolute"))
				ct = xml_misc::absolute;
			else if (coordinate == std::string("relative"))
				ct = xml_misc::relative;
			else
				throw std::runtime_error(
					"XML PARSE ERROR : coordinate can only be relative or "\
					"absolute");
		} else {
			throw std::runtime_error(
				"XML PARSE ERROR : the only attribute can be coordinate");
		}
	}
	while ((xml->getNodeType() != irr::io::EXN_ELEMENT_END) ||
		(std::string(xml->getNodeName()) != std::string("image")))
	{
		if (!xml->read())
			throw std::runtime_error(
				"XML PARSE ERROR : Unexpected end of file in parseImage");
		if ((xml->getNodeType() == irr::io::EXN_ELEMENT) &&
			(std::string(xml->getNodeName()) == std::string("position")))
		{
			if (is_position)
				throw std::runtime_error(
					"XML PARSE ERROR : only one position allowed per image");
			is_position = true;
			position = xml_misc::parsePosition(xml);
		}
		if ((xml->getNodeType() == irr::io::EXN_ELEMENT) &&
			(std::string(xml->getNodeName()) == std::string("size")))
		{
			if (is_size)
				throw std::runtime_error(
					"XML PARSE ERROR : only one (or no) size allowed per "\
					"image");
			is_size = true;
			size = xml_misc::parseSize(xml);
		}
		if ((xml->getNodeType() == irr::io::EXN_ELEMENT) &&
			(std::string(xml->getNodeName()) == std::string("location")))
		{
			if (is_location)
				throw std::runtime_error(
					"XML PARSE ERROR : only one location allowed per image");
			is_location = true;
			location = xml_misc::parseLocation(xml);
		}
	}
	if (!is_position)
		throw std::runtime_error("XML PARSE ERROR : no position in image");
	if (!is_location)
		throw std::runtime_error("XML PARSE ERROR : no location in image");
	xml_image* image = new xml_image(
		pdevice, 
		position, 
		size, 
		location,
		ct);
	return image;
}

xml_image::~xml_image() {
	m_texture->drop();
}

xml_image::xml_image(		
	irr::IrrlichtDevice* pdevice,
	const irr::core::vector2df& position,
	const irr::core::vector2df& size,
	const std::string& path,
	xml_misc::coordinate_type ct) 
	:	m_position(position),
		m_size(size),
		m_ct(ct)
{
	irr::video::IVideoDriver* pvideo = pdevice->getVideoDriver();
	irr::video::IImage* pimage = pvideo->createImageFromFile(
		getPathOfMedia(path.c_str()).c_str());
	if (!pimage) {
		std::ostringstream oss("");
		oss << "IMAGE ERROR : could not load image : " << path;
		throw std::runtime_error(oss.str());
	}
	// get some dimensions
	irr::core::dimension2d<irr::u32> image_size = pimage->getDimension();
	irr::core::dimension2d<irr::u32> screen_size = pvideo->getScreenSize();
	// convert relative coordinate into absolute
	if (m_ct == xml_misc::relative) {
		m_position.X *= (float)(screen_size.Width);
		m_position.Y *= (float)(screen_size.Height);
		m_size.X *= (float)(screen_size.Width);
		m_size.Y *= (float)(screen_size.Height);
	}
	// see if we need to resize the image
	if (m_size.X > 0.0f && m_size.Y > 0.0f) {
		if (((unsigned long)(m_size.X) != image_size.Width) ||
			((unsigned long)(m_size.Y) != image_size.Height))
		{
			// resize
			irr::core::dimension2d<irr::u32> new_image_size(
				(unsigned long)m_size.X, 
				(unsigned long)m_size.Y);
			irr::video::IImage* new_image = pvideo->createImage(
				pimage->getColorFormat(),
				new_image_size);
			// copy to new image
			pimage->copyToScaling(new_image);
			pimage->drop();
			pimage = new_image;
		}
	} else {
		m_size.X = (float)image_size.Width;
		m_size.Y = (float)image_size.Height;
	}
	m_texture = pvideo->addTexture(
		getPathOfMedia(path.c_str()).c_str(), 
		pimage);
	pimage->drop();
}

irr::s32 xml_image::addElement(
	irr::gui::IGUIEnvironment* penv, 
	irr::gui::IGUIElement* parent) 
{
	static irr::s32 id = 100;
	irr::core::position2di pos(
		(long)(m_position.X - (m_size.X / 2.0f)),
		(long)(m_position.Y - (m_size.Y / 2.0f)));
	penv->addImage(m_texture, pos, true, parent, id++);
	return id;
}
