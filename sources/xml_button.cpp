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

#include "xml_button.h"
#include "xml_misc.h"
#include "media_collection.h"
#include "parameter_set.h"
#include "sound.h"

xml_button* xml_button::parseButton(
	irr::io::IXMLReaderUTF8* xml,
	irr::IrrlichtDevice* pdevice) 
{
	bool is_position = false;
	bool is_size = false;
	bool is_image_up = false;
	bool is_image_down = false;
	bool is_image_flyby = false;
	irr::core::vector2df position(0.0f, 0.0f);
	irr::core::vector2df size(0.0f, 0.0f);
	std::string path_image_up("");
	std::string path_image_down("");
	std::string path_image_flyby("");
	std::string next("");
	xml_misc::coordinate_type ct = xml_misc::absolute;
	if (xml->getAttributeCount() > 2)
		throw std::runtime_error(
			"XML PARSE ERROR : xml attribute of button > 2");
	for (unsigned int i = 0; i < xml->getAttributeCount(); ++i) {
		std::string attrib = xml->getAttributeName(i);
		if (attrib == std::string("coordinate")) {
			std::string coordinate = xml->getAttributeValue(i);
			if (coordinate == std::string("absolute"))
				ct = xml_misc::absolute;
			else if (coordinate == std::string("relative"))
				ct = xml_misc::relative;
			else
				throw std::runtime_error(
					"XML PARSE ERROR : coordinate can only be relative or"\
					" absolute");
			continue;
		}
		if (attrib == std::string("goto-state")) {
			next = xml->getAttributeValue(i);
			continue;
		}
		throw std::runtime_error(
			"XML PARSER ERROR : unknown attribute only coordinate or "\
			"goto-state allowed in button");
	}
	if (!next.size())
		throw std::runtime_error(
			"XML PARSER ERROR : missing goto-state in button");
	while ((xml->getNodeType() != irr::io::EXN_ELEMENT_END) ||
		(std::string(xml->getNodeName()) != std::string("button")))
	{
		if (!xml->read())
			throw std::runtime_error(
				"XML PARSE ERROR : Unexpected end of file in parsebutton");
		if ((xml->getNodeType() == irr::io::EXN_ELEMENT) &&
			(std::string(xml->getNodeName()) == std::string("position")))
		{
			if (is_position)
				throw std::runtime_error(
					"XML PARSE ERROR : only one position allowed per button");
			is_position = true;
			position = xml_misc::parsePosition(xml);
		}
		if ((xml->getNodeType() == irr::io::EXN_ELEMENT) &&
			(std::string(xml->getNodeName()) == std::string("size")))
		{
			if (is_size)
				throw std::runtime_error(
					"XML PARSE ERROR : only one (or no) size allowed per"\
					" button");
			is_size = true;
			size = xml_misc::parseSize(xml);
		}
		if ((xml->getNodeType() == irr::io::EXN_ELEMENT) &&
			(std::string(xml->getNodeName()) == std::string("image-up")))
		{
			if (is_image_up)
				throw std::runtime_error(
					"XML PARSE ERROR : only one image-up allowed per button");
			is_image_up = true;
			path_image_up = xml_misc::parseImageUp(xml);
		}
		if ((xml->getNodeType() == irr::io::EXN_ELEMENT) &&
			(std::string(xml->getNodeName()) == std::string("image-down")))
		{
			if (is_image_down)
				throw std::runtime_error(
					"XML PARSE ERROR : only one image-down allowed per button");
			is_image_down = true;
			path_image_down = xml_misc::parseImageDown(xml);
		}
		if ((xml->getNodeType() == irr::io::EXN_ELEMENT) &&
			(std::string(xml->getNodeName()) == std::string("image-flyby")))
		{
			if (is_image_flyby)
				throw std::runtime_error(
					"XML PARSE ERROR : only one image-flyby allowed per"\
					" button");
			is_image_flyby = true;
			path_image_flyby = xml_misc::parseImageFlyby(xml);
		}
	}
	if (!is_position)
		throw std::runtime_error("XML PARSE ERROR : no position in button");
	if (!is_image_up)
		throw std::runtime_error("XML PARSE ERROR : no image-up in button");
	if (!is_image_down)
		throw std::runtime_error("XML PARSE ERROR : no image-down in button");
	if (!is_image_flyby)
		throw std::runtime_error("XML PARSE ERROR : no image-flyby in button");
	xml_button* button = new xml_button(
		pdevice, 
		next,
		position, 
		size, 
		path_image_up,
		path_image_down,
		path_image_flyby,
		ct);
	return button;
}

xml_button::~xml_button() {
	m_texture_up->drop();
	m_texture_down->drop();
	m_texture_flyby->drop();
}

xml_button::xml_button(
	irr::IrrlichtDevice* pdevice,
	const std::string& next,
	const irr::core::vector2df& position,
	const irr::core::vector2df& size,
	const std::string& path_up,
	const std::string& path_down,
	const std::string& path_flyby,
	xml_misc::coordinate_type ct)
	:	m_position(position),
		m_size(size),
		m_ct(ct),
		m_bst(xml_misc::bt_up),
		m_next(next),
		m_pdevice(pdevice)
{
	irr::video::IVideoDriver* pvideo = pdevice->getVideoDriver();
	irr::video::IImage* pimage_up = pvideo->createImageFromFile(
		getPathOfMedia(path_up.c_str()).c_str());
	irr::video::IImage* pimage_down = pvideo->createImageFromFile(
		getPathOfMedia(path_down.c_str()).c_str());
	irr::video::IImage* pimage_flyby = pvideo->createImageFromFile(
		getPathOfMedia(path_flyby.c_str()).c_str());
	{ // check if file well loaded
		std::ostringstream oss("");
		oss << "IMAGE ERROR : could not load image : ";
		if (!pimage_up)
			oss << path_up << " ";
		if (!pimage_down)
			oss << path_down << " ";
		if (!pimage_flyby)
			oss << path_flyby << " ";
		if (!pimage_up || !pimage_down || !pimage_flyby)
			throw std::runtime_error(oss.str());
	}
	// only get the dimension from one file (should all be the same I hope)
	irr::core::dimension2d<irr::u32> image_size = pimage_up->getDimension();
	irr::core::dimension2d<irr::u32> screen_size = pvideo->getScreenSize();
	// convert relative to absolute
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
			irr::video::IImage* new_image_up = pvideo->createImage(
				pimage_up->getColorFormat(),
				new_image_size);
			irr::video::IImage* new_image_down = pvideo->createImage(
				pimage_down->getColorFormat(),
				new_image_size);
			irr::video::IImage* new_image_flyby = pvideo->createImage(
				pimage_flyby->getColorFormat(),
				new_image_size);
			// copy to new image (up)
			pimage_up->copyToScaling(new_image_up);
			pimage_up->drop();
			pimage_up = new_image_up;
			// copy to new image (down)
			pimage_down->copyToScaling(new_image_down);
			pimage_down->drop();
			pimage_down = new_image_down;
			// copy to new image (flyby)
			pimage_flyby->copyToScaling(new_image_flyby);
			pimage_flyby->drop();
			pimage_flyby = new_image_flyby;
		}
	} else {
		m_size.X = (float)image_size.Width;
		m_size.Y = (float)image_size.Height;
	}
	m_texture_up = pvideo->addTexture(
		getPathOfMedia(path_up.c_str()).c_str(), pimage_up);
	m_texture_down = pvideo->addTexture(
		getPathOfMedia(path_down.c_str()).c_str(), pimage_down);
	m_texture_flyby = pvideo->addTexture(
		getPathOfMedia(path_flyby.c_str()).c_str(), pimage_flyby);
	pimage_up->drop();
	pimage_down->drop();
	pimage_flyby->drop();
}

bool xml_button::isIn(const irr::core::vector2d<irr::s32>& mouse) const {
	irr::core::recti button_zone(
		(irr::s32)(m_position.X - (m_size.X / 2.0f)),
		(irr::s32)(m_position.Y - (m_size.Y / 2.0f)),
		(irr::s32)(m_position.X + (m_size.X / 2.0f)),
		(irr::s32)(m_position.Y + (m_size.Y / 2.0f)));
	return button_zone.isPointInside(mouse);
}

bool xml_button::OnEvent(const irr::SEvent& event) {
	irr::core::vector2d<irr::s32> mouse_pos;
	mouse_pos.X = event.MouseInput.X;
	mouse_pos.Y = event.MouseInput.Y;
	if (event.EventType != irr::EET_MOUSE_INPUT_EVENT) return false;
	if (event.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN) {
		if (isIn(mouse_pos)) {
			if (getButtonState() != xml_misc::bt_down) {
				setButtonState(xml_misc::bt_down);
				sound::instance()->play("click");
				return true;
			}
		} else {
			if (getButtonState() != xml_misc::bt_up) {
				setButtonState(xml_misc::bt_up);
				return true;
			}
		}
	}
	if (event.MouseInput.Event == irr::EMIE_LMOUSE_LEFT_UP) {
		if (isIn(mouse_pos)) {
			if (getButtonState() == xml_misc::bt_down) {
				parameter_set::instance()->setValue(
					std::string("biolite.menu.current"), 
					getNextState());
				return true;
			}
		} else {
			if (getButtonState() != xml_misc::bt_up) {
				setButtonState(xml_misc::bt_up);
				return true;
			}
		}
	}
	if (event.MouseInput.Event == irr::EMIE_MOUSE_MOVED) {
		if (isIn(mouse_pos)) {
			if (getButtonState() != xml_misc::bt_flyby) {
				setButtonState(xml_misc::bt_flyby);
				sound::instance()->play("flyby");
				return true;
			}
		} else {
			if (getButtonState() != xml_misc::bt_up) {
				setButtonState(xml_misc::bt_up);
				return true;
			}
		}	
	}
	return false;
}

irr::s32 xml_button::addElement(
	irr::gui::IGUIEnvironment* penv, 
	irr::gui::IGUIElement* parent) 
{
	switch (m_bst) {
	case xml_misc::bt_down :
		return addImageDown(penv, parent);
	case xml_misc::bt_flyby :
		return addImageFlyby(penv, parent);
	case xml_misc::bt_up :
	default :
		return addImageUp(penv, parent);
	}
}

irr::s32 xml_button::addImageUp(
	irr::gui::IGUIEnvironment* penv, 
	irr::gui::IGUIElement* parent) 
{
	static irr::s32 id = 200;
	irr::core::position2di pos(
		(long)(m_position.X - (m_size.X / 2.0f)),
		(long)(m_position.Y - (m_size.Y / 2.0f)));
	penv->addImage(m_texture_up, pos, true, parent, ++id);
	return id;
}

irr::s32 xml_button::addImageDown(
	irr::gui::IGUIEnvironment* penv, 
	irr::gui::IGUIElement* parent) 
{
	static irr::s32 id = 300;
	irr::core::position2di pos(
		(long)(m_position.X - (m_size.X / 2.0f)),
		(long)(m_position.Y - (m_size.Y / 2.0f)));
	penv->addImage(m_texture_down, pos, true, parent, ++id);
	return id;
}

irr::s32 xml_button::addImageFlyby(
	irr::gui::IGUIEnvironment* penv, 
	irr::gui::IGUIElement* parent) 
{
	static irr::s32 id = 400;
	irr::core::position2di pos(
		(long)(m_position.X - (m_size.X / 2.0f)),
		(long)(m_position.Y - (m_size.Y / 2.0f)));
	penv->addImage(m_texture_flyby, pos, true, parent, ++id);
	return id;
}
