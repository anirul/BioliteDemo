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

#ifndef BIOLITE_XML_GAME_LOGIC_HEADER_DEFINED
#define BIOLITE_XML_GAME_LOGIC_HEADER_DEFINED

#include "game_logic.h"

class xml_game_logic : public game_logic {
public :
	//! create a xml_game_logic from an xml file
	static std::string parseGameLogic(irr::io::IXMLReaderUTF8* xml);
	//! create a xml_game_logic from a file name
	static xml_game_logic* parseGameLogic(
		const std::string& file, 
		irr::io::IFileSystem* fs);
public :
	//! contructor from parameters
	xml_game_logic(
		float update_freq,
		std::vector<float> player_energy,
		float plant_speed,
		float plant_cost,
		float plant_income,
		float fruit_cost,
		float destroy_multiply);
	//! destructor
	virtual ~xml_game_logic();
};

#endif // BIOLITE_XML_GAME_LOGIC_HEADER_DEFINED