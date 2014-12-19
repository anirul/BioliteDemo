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

#ifndef BIOLITE_XML_PLANET_HEADER_DEFINED
#define BIOLITE_XML_PLANET_HEADER_DEFINED

#include "planet.h"

class xml_planet : public planet {
public :
	//! create a xml_planet from an xml file
	static std::string parsePlanet(irr::io::IXMLReaderUTF8* xml);
	//! create a xml_planet from a file name
	static xml_planet* parsePlanet(
		const std::string& file,
		irr::scene::ISceneNode* parent,
		irr::scene::ISceneManager* mgr);
	//! write to xml file
	static void savePlanet(
		std::ostream& os,
		const planet& pl);
public :
	//! constructor from parameters
	xml_planet(
		irr::scene::ISceneNode* parent,
		irr::scene::ISceneManager* mgr,
		const std::string& seed,
		const irr::video::SColor& low,
		const irr::video::SColor& med,
		const irr::video::SColor& high,
		const std::vector<irr::video::S3DVertex>& vertices,
		const std::vector<unsigned short>& indices);
	//! destructor
	virtual ~xml_planet();
public :
	//! save to XML
	void save(const std::string& file);
};

#endif // BIOLITE_XML_PLANET_HEADER_DEFINED