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

#ifndef BIOLITE_GAME_STATE_XML_GAME_HEADER_DEFINED
#define BIOLITE_GAME_STATE_XML_GAME_HEADER_DEFINED

#include "main.h"
#include "game.h"
#include "xml_element.h"
#include "xml_game_logic.h"

class game_state_xml_game : public game_state, public parameter_set_listener {
private :
	std::string m_next_victory;
	std::string m_next_defeat;
	std::string m_name;
	std::string m_linked_to;
	std::string m_victory_on;
	std::string m_defeat_on;
	std::string m_logic_name;
	std::list<xml_element*> m_element_list;
	xml_game_logic* m_logic;
	bool m_active;
public :
	//! create a game_state_game from an xml fragment
	static game_state_xml_game* parseGameState(
		irr::io::IXMLReaderUTF8* xml,
		irr::IrrlichtDevice* pdevice);
public :
	game_state_xml_game(
		const std::string& name,
		const std::string& next_victory,
		const std::string& next_defeat,
		const std::string& logic_name,
		const std::string& linked_to,
		const std::string& victory_on,
		const std::string& defeat_on)
		:	m_next_victory(next_victory),
			m_next_defeat(next_defeat),
			m_name(name),
			m_linked_to(linked_to),
			m_victory_on(victory_on),
			m_defeat_on(defeat_on),
			m_logic_name(logic_name),
			m_logic(NULL),
			m_active(false) 
	{
		parameter_set::instance()->addListener(this);
	}
	virtual ~game_state_xml_game();
	void addElement(xml_element* element)
		{ m_element_list.push_back(element); }
	virtual const std::string getName() const { return m_name; }
	virtual void render(game* pg, irr::IrrlichtDevice* pdevice);
	virtual bool OnEvent(game* pg, const irr::SEvent& event);
	virtual void operator()(const std::string& name, const std::string& value);
};

#endif // BIOLITE_GAME_STATE_XML_GAME_HEADER_DEFINED