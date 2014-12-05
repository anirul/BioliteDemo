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

#include "xml_menu.h"
#include "parameter_set.h"
#include "media_collection.h"

xml_menu::xml_menu(
	irr::io::IXMLReaderUTF8* xml,
	irr::IrrlichtDevice* pdevice)  
{	
	while (xml->read()) {
		if (xml->getNodeType() == irr::io::EXN_ELEMENT) {
			std::string elementName = xml->getNodeName();
			// got a still-state
			if (elementName == std::string("still-state"))
				m_state_list.push_back(dynamic_cast<game_state*>(
					game_state_xml_still::parseStillState(xml, pdevice)));
			// got a gui-state
			if (elementName == std::string("gui-state"))
				m_state_list.push_back(dynamic_cast<game_state*>(
					game_state_xml_gui::parseGUIState(xml, pdevice)));
			if (elementName == std::string("game-state"))
				m_state_list.push_back(dynamic_cast<game_state*>(
					game_state_xml_game::parseGameState(xml, pdevice)));
			// set value (@ startup)
			if (elementName == std::string("set-value"))
			{
				xml_set_value* p = xml_set_value::parseSetValue(xml, pdevice);
				p->applySetValue();
				delete p;
			}
		}
		// check for the closure of menu
		if ((xml->getNodeType() == irr::io::EXN_ELEMENT_END) &&
			(std::string(xml->getNodeName()) == std::string("menu")))
			return;
	}
	throw std::runtime_error("XML PARSE ERROR : unexpected end of file");
}

int xml_menu::registerState(game* pg) {
	// register all state
	std::list<game_state*>::iterator ite;
	for (ite = m_state_list.begin(); ite != m_state_list.end(); ++ite)
		pg->registerState((*ite));
	return m_state_list.size();
}
