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

#ifndef BIOLITE_XML_MISC_HEADER_DEFINED
#define BIOLITE_XML_MISC_HEADER_DEFINED

#include "main.h"
#include "plant.h"

namespace xml_misc {

	enum button_state_type {
		bt_up = 0,
		bt_down = 1,
		bt_flyby = 2,
	};
	
	enum toggle_button_state_type {
		tb_up = 0,
		tb_up_flyby = 1,
		tb_down = 2,
		tb_down_flyby = 3
	};

	enum element_type {
		XML_BUTTON = 0,
		XML_TOGGLE_BUTTON = 1,
		XML_IMAGE = 2,
		XML_IRR_BUTTON = 10,
		XML_IRR_TEXT = 11,
		XML_IRR_LABEL = 12,
		XML_IRR_SLIDER = 13,
		XML_IRR_COMBO = 14,
		XML_SET_VALUE = 20
	};

	enum coordinate_type {
		absolute = 0,
		relative = 1
	};
	
	irr::core::vector2df parsePosition(irr::io::IXMLReaderUTF8* xml);
	irr::core::vector2df parseSize(irr::io::IXMLReaderUTF8* xml);
	irr::core::vector2df parseVector2d(irr::io::IXMLReaderUTF8* xml);
	irr::core::vector3df parseVector3d(irr::io::IXMLReaderUTF8* xml);
	irr::core::vector3di parseTriangle(irr::io::IXMLReaderUTF8* xml);
	irr::video::SColor parseColor(irr::io::IXMLReaderUTF8* xml);
	irr::video::S3DVertex parseVertex(irr::io::IXMLReaderUTF8* xml);
	plant parsePlant(irr::io::IXMLReaderUTF8* xml);
	std::pair<std::string, std::string> parseItem(
		irr::io::IXMLReaderUTF8* xml);
	std::string parseLocation(
		irr::io::IXMLReaderUTF8* xml, 
		const std::string&  tag_name = std::string("location"));
	std::string parseImageUp(irr::io::IXMLReaderUTF8* xml);
	std::string parseImageDown(irr::io::IXMLReaderUTF8* xml);
	std::string parseImageFlyby(irr::io::IXMLReaderUTF8* xml);
	std::string parseImageUpFlyby(irr::io::IXMLReaderUTF8* xml);
	std::string parseImageDownFlyby(irr::io::IXMLReaderUTF8* xml);
	
	void saveVertex(
		std::ostream& os, 
		const irr::video::S3DVertex& vert,
		unsigned int level = 0);
	void savePosition(
		std::ostream& os,
		const irr::core::vector3df& vec,
		unsigned int level = 0);
	void saveNormal(
		std::ostream& os,
		const irr::core::vector3df& vec,
		unsigned int level = 0);
	void saveTexture(
		std::ostream& os,
		const irr::core::vector2df& vec,
		unsigned int level = 0);
	void saveColor(
		std::ostream& os,
		const irr::video::SColor& col,
		unsigned int level = 0,
		const std::string& post = std::string(""));
	void saveTriangle(
		std::ostream& os,
		unsigned short A,
		unsigned short B,
		unsigned short C,
		unsigned int level = 0);
}

#endif // BIOLITE_XML_MISC_HEADER_DEFINED