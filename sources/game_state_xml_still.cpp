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

#include "game_state_xml_still.h"
#include "xml_image.h"
#include "xml_irr_label.h"
#include "xml_set_value.h"
#include "parameter_set.h"

game_state_xml_still* game_state_xml_still::parseStillState(
	irr::io::IXMLReaderUTF8* xml,
	irr::IrrlichtDevice* pdevice) 
{
	if (xml->getAttributeCount() < 2)
		throw std::runtime_error(
			"XML PARSE ERROR : xml attribute of still-state < 2");
	if (xml->getAttributeCount() > 3)
		throw std::runtime_error(
			"XML PARSE ERROR : xml attribute of still-state > 3");
	std::string name;
	std::string next;
	float timeout = 0.0f;
	for (unsigned int i = 0; i < xml->getAttributeCount(); ++i) {
		std::string attribute = xml->getAttributeName(i);
		if (attribute == std::string("name"))
			name = xml->getAttributeValue(i);
		if (attribute == std::string("goto-state"))
			next = xml->getAttributeValue("goto-state");
		if (attribute == std::string("timeout"))
			timeout = xml->getAttributeValueAsFloat("timeout");
	}
	if (!name.size())
		throw std::runtime_error(
			"XML PARSE ERROR : expected name attribute in still-state");
	if (!next.size())
		throw std::runtime_error(
		"XML PARSE ERROR : expected goto-state attribute in still-state");
	game_state_xml_still* still_state = 
		new game_state_xml_still(name, next, timeout);
	// wait for the end of still-state
	while ((xml->getNodeType() != irr::io::EXN_ELEMENT_END) ||
		(std::string(xml->getNodeName()) != std::string("still-state"))) 
	{
		if (!xml->read()) 
			throw std::runtime_error(
				"XML PARSE ERROR : Unexpected end of file in parseStillState");
		// found an image
		if ((xml->getNodeType() == irr::io::EXN_ELEMENT) &&
			(std::string(xml->getNodeName()) == std::string("image")))
			still_state->addElement(
				xml_image::parseImage(xml, pdevice));
		// found an irr label (statictext)
		if ((xml->getNodeType() == irr::io::EXN_ELEMENT) &&
			(std::string(xml->getNodeName()) == std::string("irr-label")))
			still_state->addElement(
				xml_irr_label::parseLabel(xml, pdevice));
		if ((xml->getNodeType() == irr::io::EXN_ELEMENT) &&
			(std::string(xml->getNodeName()) == std::string("set-value")))
			still_state->addElement(
				xml_set_value::parseSetValue(xml, pdevice));
	}
	return still_state;
}

game_state_xml_still::~game_state_xml_still() {
	std::list<xml_element*>::iterator ite;
	for (ite = m_element_list.begin(); ite != m_element_list.end(); ++ite)
		delete (*ite);
}

void game_state_xml_still::render(game* pg, irr::IrrlichtDevice* pdevice) {
	if (!m_active) {
		if (!m_timer) m_timer = pdevice->getTimer();
		m_start_ms = m_timer->getTime();
		pdevice->getGUIEnvironment()->clear();
		std::list<xml_element*>::iterator ite;
		for (ite = m_element_list.begin(); ite != m_element_list.end(); ++ite)
			(*ite)->addElement(
				pdevice->getGUIEnvironment(),
				pdevice->getGUIEnvironment()->getRootGUIElement());
		m_active = true;
	} else {
		if (m_timer && m_timeout != 0.0f) {
			irr::u32 millisec = m_timer->getTime();
			if ((millisec - m_start_ms) > (irr::u32)(m_timeout * 1000.0f)) {
				m_active = false;
				parameter_set::instance()->setValue(
					std::string("biolite.menu.current"),
					m_next);
			}
		}
	}
}

bool game_state_xml_still::OnEvent(game* pg, const irr::SEvent& event) {
	if (event.EventType == irr::EET_MOUSE_INPUT_EVENT) {
		if ((event.MouseInput.Event == irr::EMIE_LMOUSE_LEFT_UP) ||
			(event.MouseInput.Event == irr::EMIE_RMOUSE_LEFT_UP)) 
		{
			m_active = false;
			parameter_set::instance()->setValue(
				std::string("biolite.menu.current"),
				m_next);
			return true;
		}
	}
	if (event.EventType == irr::EET_KEY_INPUT_EVENT) {
		if (event.KeyInput.PressedDown) return false;
		if (event.KeyInput.Key == irr::KEY_ESCAPE) {
			m_active = false;
			parameter_set::instance()->setValue(
				std::string("biolite.menu.current"),
				m_next);
			return true;
		}
	}
	return false;
}
