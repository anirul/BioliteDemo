/*
 * Copyright (c) 2010, anirul
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

#include "game_state_xml_game.h"
#include "xml_image.h"
#include "xml_set_value.h"
#include "xml_button.h"
#include "xml_toggle_button.h"
#include "xml_irr_label.h"
#include "parameter_set.h"

game_state_xml_game* game_state_xml_game::parseGameState(
	irr::io::IXMLReaderUTF8* xml,
	irr::IrrlichtDevice* pdevice)
{
	if (xml->getAttributeCount() != 7)
		throw std::runtime_error(
			"XML PARSE ERROR : xml attribute of game-state != 7");
	std::string name;
	std::string next_victory;
	std::string next_defeat;
	std::string linked_to;
	std::string victory_on;
	std::string defeat_on;
	std::string logic_name;
	for (unsigned int i = 0; i < xml->getAttributeCount(); ++i) {
		std::string attrib = xml->getAttributeName(i);
		if (attrib == std::string("name")) {
			name = xml->getAttributeValue(i);
			continue;
		}
		if (attrib == std::string("goto-state-victory")) {
			next_victory = xml->getAttributeValue(i);
			continue;
		}
		if (attrib == std::string("goto-state-defeat")) {
			next_defeat = xml->getAttributeValue(i);
			continue;
		}
		if (attrib == std::string("linked-to")) {
			linked_to = xml->getAttributeValue(i);
			continue;
		}
		if (attrib == std::string("victory-on")) {
			victory_on = xml->getAttributeValue(i);
			continue;
		}
		if (attrib == std::string("defeat-on")) {
			defeat_on = xml->getAttributeValue(i);
			continue;
		}
		if (attrib == std::string("logic")) {
			logic_name = xml->getAttributeValue(i);
			continue;
		}
		{
			std::stringstream ss("");
			ss << "XML PARSE ERROR : unknown attribute in game-state : ";
			ss << xml->getAttributeName(i);
			throw std::runtime_error(ss.str().c_str());
		}
	}
	if (!name.size())
		throw std::runtime_error(
			"XML PARSE ERROR : expected name in game-state");
	if (!next_victory.size())
		throw std::runtime_error(
			"XML PARSE ERROR : expected goto-state-victory in game-state");
	if (!next_defeat.size())
		throw std::runtime_error(
			"XML PARSE ERROR : expected goto-state-defeat in game-state");
	if (!linked_to.size())
		throw std::runtime_error(
			"XML PARSE ERROR : expected linked-to in game-state");
	if (!victory_on.size())
		throw std::runtime_error(
			"XML PARSE ERORR : expected victory-on in game-state");
	if (!defeat_on.size())
		throw std::runtime_error(
			"XML PARSE ERORR : expected defeat-on in game-state");
	game_state_xml_game* state = new game_state_xml_game(
		name, 
		next_victory, 
		next_defeat,
		logic_name,
		linked_to, 
		victory_on,
		defeat_on);
	while ((xml->getNodeType() != irr::io::EXN_ELEMENT_END) ||
		(std::string(xml->getNodeName()) != std::string("game-state")))
	{
		if (!xml->read())
			throw std::runtime_error(
				"XML PARSE ERROR : Unexpected end of file in parseGameState");
		if (xml->getNodeType() == irr::io::EXN_ELEMENT) {
			// found an image
			if (std::string(xml->getNodeName()) == std::string("image")) {
				state->addElement(
					xml_image::parseImage(xml, pdevice));
				continue;
			}
			// found a set value
			if (std::string(xml->getNodeName()) == std::string("set-value")) {
				state->addElement(
					xml_set_value::parseSetValue(xml, pdevice));
				continue;
			}
			// found a button
			if (std::string(xml->getNodeName()) == std::string("button")) {
				state->addElement(
					xml_button::parseButton(xml, pdevice));
				continue;
			}
			// found a toggle button
			if (std::string(xml->getNodeName()) == 
				std::string("toggle-button")) 
			{
				state->addElement(
					xml_toggle_button::parseToggleButton(xml, pdevice));
				continue;
			}
			// found an irr label (statictext)
			if (std::string(xml->getNodeName()) == std::string("irr-label")) {
				state->addElement(
					xml_irr_label::parseLabel(xml, pdevice));
				continue;
			}
			std::ostringstream oss("");
			oss << "XML PARSE ERROR : Uknown element in Game state : ";
			oss << xml->getNodeName();
			throw std::runtime_error(oss.str().c_str());
		}
		// FIXME
//		throw std::runtime_error(
//			"XML PARSE ERROR : Unexpected element in Game state");
	}
	return state;
}

void game_state_xml_game::operator()(
	const std::string& name, 
	const std::string& value)
{
	m_active = false;
}

game_state_xml_game::~game_state_xml_game() {
	std::list<xml_element*>::iterator ite;
	for (ite = m_element_list.begin(); ite != m_element_list.end(); ++ite)
		delete (*ite);
	parameter_set::instance()->removeListener(this);
}

void game_state_xml_game::render(game* pg, irr::IrrlichtDevice* pdevice) {
	if (!m_logic) {
		m_logic = xml_game_logic::parseGameLogic(
			m_logic_name, 
			pdevice->getFileSystem());
	} else {
		m_logic->render(pg, pdevice);
	}
	if (!m_active) {
		pdevice->getGUIEnvironment()->clear();
		{ // add element to the list
			std::list<xml_element*>::iterator ite;
			for (ite = m_element_list.begin(); 
				ite != m_element_list.end(); 
				++ite)
			{
				(*ite)->addElement(
					pdevice->getGUIEnvironment(),
					pdevice->getGUIEnvironment()->getRootGUIElement());
			}
		}
		m_active = true;
	}
	{ // check for end conditions
		std::string value = parameter_set::instance()->getValue(m_linked_to);
		if (value == m_defeat_on) {
			m_active = false;
			delete m_logic;
			m_logic = NULL;
			parameter_set::instance()->setValue(
				std::string("biolite.menu.current"),
				m_next_defeat);
		}
		if (value == m_victory_on) {
			m_active = false;
			delete m_logic;
			m_logic = NULL;
			parameter_set::instance()->setValue(
				std::string("biolite.menu.current"),
				m_next_victory);
		}
	}
}

bool game_state_xml_game::OnEvent(game* pg, const irr::SEvent& event) {
	//! reset the timer
	if (event.EventType == irr::EET_MOUSE_INPUT_EVENT) {
		irr::core::vector2d<irr::s32> mouse_pos(
			event.MouseInput.X, 
			event.MouseInput.Y);
		irr::scene::ISceneManager* scene = pg->m_pdevice->getSceneManager();
		irr::scene::ISceneCollisionManager* collision = 
			scene->getSceneCollisionManager();
		irr::core::line3d<irr::f32> ray = 
			collision->getRayFromScreenCoordinates(
				mouse_pos, 
				scene->getActiveCamera());
		irr::core::vector3df hit;
		bool planet_hit = pg->m_pworld->intersect(ray, hit);
		if (event.MouseInput.Event == irr::EMIE_LMOUSE_LEFT_UP) {
			if (planet_hit) {
				std::string type = parameter_set::instance()->getValue(
					"biolite.action.type");
				click_desc cd;
				cd.m_click_t = click_desc::ct_leaf_tree;
				if (type == std::string("fruit"))
					cd.m_click_t = click_desc::ct_fruit_tree;
				if (type == std::string("destroy"))
					cd.m_click_t = click_desc::ct_destroy;
				cd.m_position = hit;
				cd.m_player_id = 0;
				if (m_logic) m_logic->planetClick(cd);
			}
		}
		if (event.MouseInput.Event == irr::EMIE_MOUSE_MOVED) {
			if (m_logic) m_logic->planetFlyby(hit);
		}
	}
	std::list<xml_element*>::iterator ite;
	for (ite = m_element_list.begin(); ite != m_element_list.end(); ++ite) {
		xml_element* element = (*ite);
		if (element->OnEvent(event)) {
			m_active = false;
			return true;
		}
	}
	if (event.EventType == irr::EET_KEY_INPUT_EVENT) {
		if (event.KeyInput.PressedDown) return false;
		if (event.KeyInput.Key == irr::KEY_ESCAPE) {
			m_active = false;
			delete m_logic;
			m_logic = NULL;
			parameter_set::instance()->setValue(
				std::string("biolite.menu.current"),
				m_next_defeat);
			return true;
		}
	}
	return false;
}