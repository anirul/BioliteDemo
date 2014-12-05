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

#include "xml_toggle_button.h"
#include "xml_misc.h"
#include "media_collection.h"
#include "parameter_set.h"
#include "sound.h"

xml_toggle_button* xml_toggle_button::parseToggleButton(
	irr::io::IXMLReaderUTF8* xml,
	irr::IrrlichtDevice* pdevice) 
{
	bool is_position = false;
	bool is_size = false;
	bool is_image_up = false;
	bool is_image_down = false;
	bool is_image_up_flyby = false;
	bool is_image_down_flyby = false;
	irr::core::vector2df position(0.0f, 0.0f);
	irr::core::vector2df size(0.0f, 0.0f);
	std::string path_image_up("");
	std::string path_image_down("");
	std::string path_image_up_flyby("");
	std::string path_image_down_flyby("");
	std::string linked_to("");
	std::string value("");
	xml_misc::coordinate_type ct = xml_misc::absolute;
	if (xml->getAttributeCount() > 3)
		throw std::runtime_error("XML PARSE ERROR : xml attribute of toggle"\
			" button > 3");
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
		if (attrib == std::string("linked-to")) {
			linked_to = xml->getAttributeValue(i);
			continue;
		}
		if (attrib == std::string("value")) {
			value = xml->getAttributeValue(i);
			continue;
		}
		throw std::runtime_error(
			"XML PARSER ERROR : unknown attribute only coordinate or"\
			" goto-state allowed in toggle button");
	}
	if (!linked_to.size())
		throw std::runtime_error(
			"XML PARSER ERROR : missing linked-to in toggle button");
	if (!value.size())
		throw std::runtime_error(
			"XML PARSER ERROR : missing value in toggle button");
	while ((xml->getNodeType() != irr::io::EXN_ELEMENT_END) ||
		(std::string(xml->getNodeName()) != std::string("toggle-button")))
	{
		if (!xml->read())
			throw std::runtime_error(
				"XML PARSE ERROR : Unexpected end of file in parsebutton");
		if ((xml->getNodeType() == irr::io::EXN_ELEMENT) &&
			(std::string(xml->getNodeName()) == std::string("position")))
		{
			if (is_position)
				throw std::runtime_error(
					"XML PARSE ERROR : only one position allowed per toggle"\
					" button");
			is_position = true;
			position = xml_misc::parsePosition(xml);
		}
		if ((xml->getNodeType() == irr::io::EXN_ELEMENT) &&
			(std::string(xml->getNodeName()) == std::string("size")))
		{
			if (is_size)
				throw std::runtime_error(
					"XML PARSE ERROR : only one (or no) size allowed per"\
					" toggle button");
			is_size = true;
			size = xml_misc::parseSize(xml);
		}
		if ((xml->getNodeType() == irr::io::EXN_ELEMENT) &&
			(std::string(xml->getNodeName()) == std::string("image-up")))
		{
			if (is_image_up)
				throw std::runtime_error(
					"XML PARSE ERROR : only one image-up allowed per toggle"\
					" button");
			is_image_up = true;
			path_image_up = xml_misc::parseImageUp(xml);
		}
		if ((xml->getNodeType() == irr::io::EXN_ELEMENT) &&
			(std::string(xml->getNodeName()) == std::string("image-down")))
		{
			if (is_image_down)
				throw std::runtime_error(
					"XML PARSE ERROR : only one image-down allowed per toggle"\
					" button");
			is_image_down = true;
			path_image_down = xml_misc::parseImageDown(xml);
		}
		if ((xml->getNodeType() == irr::io::EXN_ELEMENT) &&
			(std::string(xml->getNodeName()) == std::string("image-up-flyby")))
		{
			if (is_image_up_flyby)
				throw std::runtime_error(
					"XML PARSE ERROR : only one image-flyby allowed per toggle"\
					" button");
			is_image_up_flyby = true;
			path_image_up_flyby = xml_misc::parseImageUpFlyby(xml);
		}
		if ((xml->getNodeType() == irr::io::EXN_ELEMENT) &&
			(std::string(xml->getNodeName()) == 
			std::string("image-down-flyby")))
		{
			if (is_image_down_flyby)
				throw std::runtime_error(
					"XML PARSE ERROR : only one image-down-flyby allowed per"\
					" toggle button");
			is_image_down_flyby = true;
			path_image_down_flyby = xml_misc::parseImageDownFlyby(xml);
		}
	}
	if (!is_position)
		throw std::runtime_error(
			"XML PARSE ERROR : no position in toggle button");
	if (!is_image_up)
		throw std::runtime_error(
			"XML PARSE ERROR : no image-up in toggle button");
	if (!is_image_down)
		throw std::runtime_error(
			"XML PARSE ERROR : no image-down in toggle button");
	if (!is_image_up_flyby)
		throw std::runtime_error(
			"XML PARSE ERROR : no image-flyby in toggle button");
	xml_toggle_button* button = new xml_toggle_button(
		pdevice, 
		linked_to,
		value,
		position, 
		size, 
		path_image_up,
		path_image_down,
		path_image_up_flyby,
		path_image_down_flyby,
		ct);
	return button;
}

xml_toggle_button::~xml_toggle_button() {
	m_texture_up->drop();
	m_texture_down->drop();
	m_texture_up_flyby->drop();
	m_texture_down_flyby->drop();
	parameter_set::instance()->removeListener(this);
}

xml_toggle_button::xml_toggle_button(
	irr::IrrlichtDevice* pdevice,
	const std::string& linked_to,
	const std::string& value,
	const irr::core::vector2df& position,
	const irr::core::vector2df& size,
	const std::string& path_up,
	const std::string& path_down,
	const std::string& path_up_flyby,
	const std::string& path_down_flyby,
	xml_misc::coordinate_type ct)
	:	m_position(position),
		m_size(size),
		m_ct(ct),
		m_bst(xml_misc::tb_up),
		m_linked_to(linked_to),
		m_value(value),
		m_pdevice(pdevice)
{
	irr::video::IVideoDriver* pvideo = pdevice->getVideoDriver();
	irr::video::IImage* pimage_up = pvideo->createImageFromFile(
		getPathOfMedia(path_up.c_str()).c_str());
	irr::video::IImage* pimage_down = pvideo->createImageFromFile(
		getPathOfMedia(path_down.c_str()).c_str());
	irr::video::IImage* pimage_up_flyby = pvideo->createImageFromFile(
		getPathOfMedia(path_up_flyby.c_str()).c_str());
	irr::video::IImage* pimage_down_flyby = pvideo->createImageFromFile(
		getPathOfMedia(path_down_flyby.c_str()).c_str());
	{ // check if file well loaded
		std::ostringstream oss("");
		oss << "IMAGE ERROR : could not load image : ";
		if (!pimage_up)
			oss << path_up << " ";
		if (!pimage_down)
			oss << path_down << " ";
		if (!pimage_up_flyby)
			oss << path_up_flyby << " ";
		if (!pimage_down_flyby)
			oss << path_down_flyby << " ";
		if (!pimage_up || !pimage_down || 
			!pimage_up_flyby || !pimage_down_flyby)
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
			irr::video::IImage* new_image_up_flyby = pvideo->createImage(
				pimage_up_flyby->getColorFormat(),
				new_image_size);
			irr::video::IImage* new_image_down_flyby = pvideo->createImage(
				pimage_down_flyby->getColorFormat(),
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
			pimage_up_flyby->copyToScaling(new_image_up_flyby);
			pimage_up_flyby->drop();
			pimage_up_flyby = new_image_up_flyby;
			// copy to new image (down flyby)
			pimage_down_flyby->copyToScaling(new_image_down_flyby);
			pimage_down_flyby->drop();
			pimage_down_flyby = new_image_down_flyby;
		}
	} else {
		m_size.X = (float)image_size.Width;
		m_size.Y = (float)image_size.Height;
	}
	m_texture_up = pvideo->addTexture(
		getPathOfMedia(path_up.c_str()).c_str(), pimage_up);
	m_texture_down = pvideo->addTexture(
		getPathOfMedia(path_down.c_str()).c_str(), pimage_down);
	m_texture_up_flyby = pvideo->addTexture(
		getPathOfMedia(path_up_flyby.c_str()).c_str(), pimage_up_flyby);
	m_texture_down_flyby = pvideo->addTexture(
		getPathOfMedia(path_down_flyby.c_str()).c_str(), pimage_down_flyby);
	pimage_up->drop();
	pimage_down->drop();
	pimage_up_flyby->drop();
	pimage_down_flyby->drop();
	parameter_set::instance()->addListener(this);
}

bool xml_toggle_button::isIn(const irr::core::vector2d<irr::s32>& mouse) const {
	irr::core::recti button_zone(
		(irr::s32)(m_position.X - (m_size.X / 2.0f)),
		(irr::s32)(m_position.Y - (m_size.Y / 2.0f)),
		(irr::s32)(m_position.X + (m_size.X / 2.0f)),
		(irr::s32)(m_position.Y + (m_size.Y / 2.0f)));
	return button_zone.isPointInside(mouse);
}

bool xml_toggle_button::OnEvent(const irr::SEvent& event) {
	irr::core::vector2d<irr::s32> mouse_pos;
	mouse_pos.X = event.MouseInput.X;
	mouse_pos.Y = event.MouseInput.Y;
	// check if the value is actually selected
	if (event.EventType != irr::EET_MOUSE_INPUT_EVENT) return false;
	if (event.MouseInput.Event == irr::EMIE_LMOUSE_LEFT_UP) {
		if (isIn(mouse_pos)) {
			if (getButtonState() != xml_misc::tb_down_flyby) {
				setButtonState(xml_misc::tb_down_flyby);
				parameter_set::instance()->setValue(m_linked_to, m_value);
				sound::instance()->play("click");
				return true;
			}
		} 
	}
	if (event.MouseInput.Event == irr::EMIE_MOUSE_MOVED) {
		if (parameter_set::instance()->getValue(m_linked_to) == m_value) {
			if (isIn(mouse_pos)) {
				if (getButtonState() != xml_misc::tb_down_flyby) {
					setButtonState(xml_misc::tb_down_flyby);
					sound::instance()->play("flyby");
					return true;
				}
			} else {
				if (getButtonState() != xml_misc::tb_down) {
					setButtonState(xml_misc::tb_down);
					return true;
				}
			}
		} else {
			if (isIn(mouse_pos)) {
				if (getButtonState() != xml_misc::tb_up_flyby) {
					setButtonState(xml_misc::tb_up_flyby);
					sound::instance()->play("flyby");
					return true;
				}
			} else {
				if (getButtonState() != xml_misc::tb_up) {
					setButtonState(xml_misc::tb_up);
					return true;
				}
			}

		}
	}
	return false;
}

void xml_toggle_button::operator()(
	const std::string& name, 
	const std::string& value)
{
	if (name == m_linked_to) {
		if (value != m_value) {
			setButtonState(xml_misc::tb_up);
		}
	}
}

irr::s32 xml_toggle_button::addElement(
	irr::gui::IGUIEnvironment* penv, 
	irr::gui::IGUIElement* parent) 
{
	switch (m_bst) {
	case xml_misc::tb_down :
		return addImageDown(penv, parent);
	case xml_misc::tb_up_flyby :
		return addImageUpFlyby(penv, parent);
	case xml_misc::tb_down_flyby :
		return addImageDownFlyby(penv, parent);
	case xml_misc::tb_up :
	default :
		return addImageUp(penv, parent);
	}
}

irr::s32 xml_toggle_button::addImageUp(
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

irr::s32 xml_toggle_button::addImageDown(
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

irr::s32 xml_toggle_button::addImageUpFlyby(
	irr::gui::IGUIEnvironment* penv, 
	irr::gui::IGUIElement* parent) 
{
	static irr::s32 id = 400;
	irr::core::position2di pos(
		(long)(m_position.X - (m_size.X / 2.0f)),
		(long)(m_position.Y - (m_size.Y / 2.0f)));
	penv->addImage(m_texture_up_flyby, pos, true, parent, ++id);
	return id;
}

irr::s32 xml_toggle_button::addImageDownFlyby(
	irr::gui::IGUIEnvironment* penv,
	irr::gui::IGUIElement* parent)
{
	static irr::s32 id = 500;
	irr::core::position2di pos(
		(long)(m_position.X - (m_size.X / 2.0f)),
		(long)(m_position.Y - (m_size.Y / 2.0f)));
	penv->addImage(m_texture_down_flyby, pos, true, parent, ++id);
	return id;
}
