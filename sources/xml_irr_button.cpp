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

#include "xml_irr_button.h"
#include "xml_misc.h"
#include "parameter_set.h"

xml_irr_button* xml_irr_button::parseButton(
	irr::io::IXMLReaderUTF8* xml,
	irr::IrrlichtDevice* pdevice)
{
	bool is_position = false;
	bool is_size = false;
	irr::core::vector2df position(0.0f, 0.0f);
	irr::core::vector2df size(0.0f, 0.0f);
	std::string next("");
	std::string text("");
	std::string tooltip("");
	xml_misc::coordinate_type ct = xml_misc::absolute;
	if (xml->getAttributeCount() > 4)
		throw std::runtime_error(
			"XML PARSE ERROR : xml attribute of irr-button > 4");
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
		if (attrib == std::string("text")) {
			text = xml->getAttributeValue(i);
			continue;
		}
		if (attrib == std::string("tooltip")) {
			tooltip = xml->getAttributeValue(i);
			continue;
		}
		throw std::runtime_error(
			"XML PARSER ERROR : unknown attribute only coordinate text "\
			"goto-state and tooltip allowed in irr-button");
	}
	if (!next.size())
		throw std::runtime_error(
			"XML PARSER ERROR : missing goto-state in irr-button");
	if (!text.size())
		throw std::runtime_error(
			"XML PARSER ERROR : missing text in irr-button");
	while ((xml->getNodeType() != irr::io::EXN_ELEMENT_END) ||
		(std::string(xml->getNodeName()) != std::string("irr-button")))
	{
		if (!xml->read())
			throw std::runtime_error(
				"XML PARSE ERROR : Unexpected end of file in "\
				"xml_irr_button::parseButton");
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
					"XML PARSE ERROR : only one (or no) size allowed per "\
					"irr-button");
			is_size = true;
			size = xml_misc::parseSize(xml);
		}
	}
	if (!is_position)
		throw std::runtime_error(
			"XML PARSE ERROR : no position in irr-button");
	xml_irr_button* button = new xml_irr_button(
		pdevice, 
		next,
		position, 
		size, 
		text,
		tooltip,
		ct);
	return button;
}

xml_irr_button::~xml_irr_button() {
	if (m_button) m_button->drop();
}

xml_irr_button::xml_irr_button(
	irr::IrrlichtDevice* pdevice,
	const std::string& next,
	const irr::core::vector2df& position,
	const irr::core::vector2df& size,
	const std::string& text,
	const std::string& tooltip,
	xml_misc::coordinate_type ct)
	:	m_button(NULL),
		m_position(position),
		m_size(size),
		m_ct(ct),
		m_next(next),
		m_text(text),
		m_tooltip(tooltip)
{
	irr::video::IVideoDriver* pvideo = pdevice->getVideoDriver();
	irr::core::dimension2d<irr::u32> screen_size = pvideo->getScreenSize();
	// convert relative to absolute
	if (m_ct == xml_misc::relative) {
		m_position.X *= (float)(screen_size.Width);
		m_position.Y *= (float)(screen_size.Height);
		m_size.X *= (float)(screen_size.Width);
		m_size.Y *= (float)(screen_size.Height);
	}
}

bool xml_irr_button::isIn(const irr::core::vector2d<irr::s32>& mouse) const {
	irr::core::recti button_zone(
		(irr::s32)(m_position.X - (m_size.X / 2.0f)),
		(irr::s32)(m_position.Y - (m_size.Y / 2.0f)),
		(irr::s32)(m_position.X + (m_size.X / 2.0f)),
		(irr::s32)(m_position.Y + (m_size.Y / 2.0f)));
	return button_zone.isPointInside(mouse);
}

irr::s32 xml_irr_button::addElement(
	irr::gui::IGUIEnvironment* penv,
	irr::gui::IGUIElement* parent)
{
	static irr::s32 id = 500;
	std::wstring wtext(m_text.begin(), m_text.end());
	std::wstring wtooltip(m_tooltip.begin(), m_tooltip.end());
	irr::core::recti button_zone(
		(irr::s32)(m_position.X - (m_size.X / 2.0f)),
		(irr::s32)(m_position.Y - (m_size.Y / 2.0f)),
		(irr::s32)(m_position.X + (m_size.X / 2.0f)),
		(irr::s32)(m_position.Y + (m_size.Y / 2.0f)));
	m_button = penv->addButton(
		button_zone, 
		parent, 
		++id, 
		wtext.c_str(), 
		wtooltip.c_str());
	return id;
}

bool xml_irr_button::OnEvent(const irr::SEvent& event) {
	if (event.EventType != irr::EET_GUI_EVENT) return false;
	if (event.GUIEvent.EventType != irr::gui::EGET_BUTTON_CLICKED) return false;
	if (event.GUIEvent.Caller != m_button) return false;
	parameter_set::instance()->setValue(
		std::string("biolite.menu.current"),
		m_next);
	return true;
}