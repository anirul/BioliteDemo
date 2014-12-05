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

#ifndef BIOLITE_GAME_STATE_XML_STILL_HEADER_DEFINED
#define BIOLITE_GAME_STATE_XML_STILL_HEADER_DEFINED

#include "main.h"
#include "game.h"
#include "xml_element.h"

class game_state_xml_still : public game_state {
private:
	std::string m_next;
	std::string m_name;
	float m_timeout;
	std::list<xml_element*> m_element_list;
	bool m_active;
	irr::ITimer* m_timer;
	irr::u32 m_start_ms;
public:
	//! create a game_state_still from an xml fragment
	static game_state_xml_still* parseStillState(
		irr::io::IXMLReaderUTF8* xml,
		irr::IrrlichtDevice* pdevice);
public :
	game_state_xml_still(
		const std::string& name,
		const std::string& next,
		float timeout = 0.0f) 
		:	m_next(next),
			m_name(name),
			m_timeout(timeout),
			m_active(false),
			m_timer(NULL) {}
	virtual ~game_state_xml_still();
	void addElement(xml_element* element) 
		{ m_element_list.push_back(element); }
	virtual const std::string getName() const { return m_name; }
	virtual void render(game* pg, irr::IrrlichtDevice* pdevice);
	virtual bool OnEvent(game* pg, const irr::SEvent& event);
};

#endif // BIOLITE_GAME_STATE_XML_STILL_HEADER_DEFINED