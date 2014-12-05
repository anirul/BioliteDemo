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

#include "media_collection.h"
#include "xml_misc.h"
#include "xml_planet.h"

std::string xml_planet::parsePlanet(irr::io::IXMLReaderUTF8* xml) {
	bool fault = true;
	std::string file("");
	if (xml->getAttributeCount() > 0) 
		throw std::runtime_error(
			"XML PARSE ERROR : there is no attributes in planet");
	do {
		if (xml->getNodeType() == irr::io::EXN_ELEMENT_END)
			throw std::runtime_error("XML PARSE ERROR : this node has no text");
		if (xml->getNodeType() == irr::io::EXN_TEXT) {
			file = xml->getNodeData();
			if (!file.size())
				throw std::runtime_error(
					"XML PARSE ERROR : planet cannot be empty");
			fault = false;
			break;
		}
	} while (xml->read());
	if (fault)
		throw std::runtime_error(
			"XML PARSE ERROR : unexpected end of file in planet");
	return file;
}

xml_planet* xml_planet::parsePlanet(
	const std::string& file,
	irr::scene::ISceneNode* parent,
	irr::scene::ISceneManager* mgr)
{
	// planet XML loading
	irr::io::IXMLReaderUTF8* xml = 
		mgr->getFileSystem()->createXMLReaderUTF8(
			getPathOfMedia(file.c_str(), "xml").c_str());
	if (!xml) {
		std::ostringstream oss;
		oss << "ERROR : [" << file << "] not found!";
		throw std::runtime_error(oss.str());
	}
	// parse the planet file
	std::vector<irr::video::S3DVertex> vert_list;
	std::vector<unsigned short> index_list;
	std::string seed = "";
	irr::video::SColor low;
	irr::video::SColor medium;
	irr::video::SColor high;
	bool is_color_low = false;
	bool is_color_medium = false;
	bool is_color_high = false;
	while (xml->read()) {
		if (xml->getNodeType() == irr::io::EXN_ELEMENT) {
			std::string elementName = xml->getNodeName();
			// got the root planet
			if (elementName == std::string("planet")) {
				if (xml->getAttributeCount() != 1) throw std::runtime_error(
					"XML PARSE ERROR : planet has one attribute");
				for (unsigned int i = 0; i < xml->getAttributeCount(); ++i) {
					std::string attrib = xml->getAttributeName(i);
					if (attrib == std::string("seed")) continue;
					throw std::runtime_error(
						"XML PARSER ERROR : unknown attribute in planet");
				}
				seed = xml->getAttributeValue("seed");
			}
			// got a vertex
			if (elementName == std::string("vertex"))
				vert_list.push_back(
					xml_misc::parseVertex(xml));
			// got a triangle
			if (elementName == std::string("triangle")) {
				irr::core::vector3di vec = xml_misc::parseTriangle(xml);
				index_list.push_back(vec.X);
				index_list.push_back(vec.Y);
				index_list.push_back(vec.Z);
			}
			// got a color
			if (elementName == std::string("color-low")) {
				if (is_color_low) throw std::runtime_error(
					"XML PARSER ERROR : only one color-low per planet");
				is_color_low = true;
				low = xml_misc::parseColor(xml);
			}
			if (elementName == std::string("color-medium")) {
				if (is_color_medium) throw std::runtime_error(
					"XML PARSER ERROR : only one color-medium per planet");
				is_color_medium = true;
				medium = xml_misc::parseColor(xml);
			}
			if (elementName == std::string("color-high")) {
				if (is_color_high) throw std::runtime_error(
					"XML PARSER ERROR : only one color-high per planet");
				is_color_high = true;
				high = xml_misc::parseColor(xml);
			}
		}
		// check for the closure of planet
		if ((xml->getNodeType() == irr::io::EXN_ELEMENT_END) &&
			(std::string(xml->getNodeName()) == std::string("planet"))) 
		{
			xml_planet* pl = new xml_planet(
				parent, 
				mgr, 
				seed,
				low,
				medium,
				high,
				vert_list,
				index_list); 
			return pl;
		}
	}
	throw std::runtime_error("XML PARSE ERROR : unexpected end of file");
}

void xml_planet::savePlanet(
	std::ostream& os,
	const planet& pl) 
{
	os << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl;
	os << "<planet seed=\"" << pl.m_seed << "\">" << std::endl;
	xml_misc::saveColor(os, pl.m_ground, 1, std::string("-low"));
	xml_misc::saveColor(os, pl.m_grass, 1, std::string("-medium"));
	xml_misc::saveColor(os, pl.m_high, 1, std::string("-high"));
	for (unsigned int i = 0; i < pl.m_vvert.size(); ++i) {
		xml_misc::saveVertex(os, pl.m_vvert[i], 1);
	}
	for (unsigned int i = 0; i < pl.m_vind.size(); i += 3) {
		xml_misc::saveTriangle(
			os, 
			pl.m_vind[i], 
			pl.m_vind[i + 1], 
			pl.m_vind[i + 2], 
			1);
	}
	os << "</planet>" << std::endl;	
}

xml_planet::xml_planet(
	irr::scene::ISceneNode* parent,
	irr::scene::ISceneManager* mgr,
	const std::string& seed,
	const irr::video::SColor& low,
	const irr::video::SColor& med,
	const irr::video::SColor& high,
	const std::vector<irr::video::S3DVertex>& vertices,
	const std::vector<unsigned short>& indices)
	:	planet(parent, mgr, seed, low, med, high)
{
	m_vvert.clear();
	m_vind.clear();
	m_vvert = vertices;
	m_vind = indices;
	m_seed = seed;
	set_seed(hash(seed));
	m_gen_done = m_gen_size;
	m_normal_done = m_vvert.size();
	compute_color();
}

xml_planet::~xml_planet() {}
