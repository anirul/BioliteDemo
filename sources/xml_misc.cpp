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

#include "xml_misc.h"

irr::core::vector2df xml_misc::parsePosition(irr::io::IXMLReaderUTF8* xml) {
	if ((xml->getAttributeCount() < 2) ||
		(xml->getAttributeCount() > 2))
		throw std::runtime_error(
			"XML PARSE ERROR : position should have 2 attributes");
	irr::core::vector2df pos;
	for (unsigned int i = 0; i < xml->getAttributeCount(); i++) {
		std::string attrib = xml->getAttributeName(i);
		if (attrib == std::string("hpos")) continue;
		if (attrib == std::string("vpos")) continue;
		throw std::runtime_error(
			"XML PARSE ERROR : wrong attribute in position");
	}
	if (std::string(xml->getAttributeValue("hpos")) == std::string(""))
		throw std::runtime_error(
			"XML PARSE ERROR : position should have [hpos] attribute");
	if (std::string(xml->getAttributeValue("vpos")) == std::string(""))
		throw std::runtime_error(
			"XML PARSE ERROR : position should have [vpos] attribute");
	pos.X = xml->getAttributeValueAsFloat("hpos");
	pos.Y = xml->getAttributeValueAsFloat("vpos");
	return pos;
}

irr::core::vector2df xml_misc::parseSize(irr::io::IXMLReaderUTF8* xml) {
	if (xml->getAttributeCount() != 2)
		throw std::runtime_error(
			"XML PARSE ERROR : size should have 2 attributes");
	irr::core::vector2df pos;
	for (unsigned int i = 0; i < xml->getAttributeCount(); i++) {
		std::string attrib = xml->getAttributeName(i);
		if (attrib == std::string("width")) continue;
		if (attrib == std::string("height")) continue;
		throw std::runtime_error(
			"XML PARSE ERROR : wrong attribute in size");
	}
	if (std::string(xml->getAttributeValue("width")) == std::string(""))
		throw std::runtime_error(
			"XML PARSE ERROR : size should have [width] attribute");
	if (std::string(xml->getAttributeValue("height")) == std::string(""))
		throw std::runtime_error(
			"XML PARSE ERROR : size should have [height] attribute");
	pos.X = xml->getAttributeValueAsFloat("width");
	pos.Y = xml->getAttributeValueAsFloat("height");
	return pos;
}

irr::core::vector2df xml_misc::parseVector2d(irr::io::IXMLReaderUTF8* xml) {
	if (xml->getAttributeCount() != 2)
		throw std::runtime_error(
			"XML PARSE ERROR : vector2d should have 2 attributes");
	irr::core::vector2df vec;
	for (unsigned int i = 0; i < xml->getAttributeCount(); ++i) {
		std::string attrib = xml->getAttributeName(i);
		if (attrib == std::string("X")) continue;
		if (attrib == std::string("Y")) continue;
		throw std::runtime_error(
			"XML PARSE ERROR : wrong attribute in vector2d");
	}
	if (std::string(xml->getAttributeValue("X")) == std::string(""))
		throw std::runtime_error(
			"XML PARSE ERROR : vector2d should have a [X] attribute");
	if (std::string(xml->getAttributeValue("Y")) == std::string(""))
		throw std::runtime_error(
			"XML PARSE ERROR : vector2d should have a [Y] attribute");
	vec.X = xml->getAttributeValueAsFloat("X");
	vec.Y = xml->getAttributeValueAsFloat("Y");
	return vec;
}

irr::core::vector3df xml_misc::parseVector3d(irr::io::IXMLReaderUTF8* xml) {
	if (xml->getAttributeCount() != 3)
		throw std::runtime_error(
			"XML PARSE ERROR : vector3d should have 3 attributes");
	irr::core::vector3df vec;
	for (unsigned int i = 0; i < xml->getAttributeCount(); ++i) {
		std::string attrib = xml->getAttributeName(i);
		if (attrib == std::string("X")) continue;
		if (attrib == std::string("Y")) continue;
		if (attrib == std::string("Z")) continue;
		throw std::runtime_error(
			"XML PARSE ERROR : wrong attribute in vector3d");
	}
	if (std::string(xml->getAttributeValue("X")) == std::string(""))
		throw std::runtime_error(
			"XML PARSE ERROR : vector3d should have a [X] attribute");
	if (std::string(xml->getAttributeValue("Y")) == std::string(""))
		throw std::runtime_error(
			"XML PARSE ERROR : vector3d should have a [Y] attribute");
	if (std::string(xml->getAttributeValue("Z")) == std::string(""))
		throw std::runtime_error(
			"XML PARSE ERROR : vector3d should have a [Z] attribute");
	vec.X = xml->getAttributeValueAsFloat("X");
	vec.Y = xml->getAttributeValueAsFloat("Y");
	vec.Z = xml->getAttributeValueAsFloat("Z");
	return vec;
}

irr::core::vector3di xml_misc::parseTriangle(irr::io::IXMLReaderUTF8* xml) {
	if (xml->getAttributeCount() != 3)
		throw std::runtime_error(
			"XML PARSE ERROR : triangle should have 3 attributes");
	irr::core::vector3di vec;
	for (unsigned int i = 0; i < xml->getAttributeCount(); ++i) {
		std::string attrib = xml->getAttributeName(i);
		if (attrib == std::string("A")) continue;
		if (attrib == std::string("B")) continue;
		if (attrib == std::string("C")) continue;
		throw std::runtime_error(
			"XML PARSE ERROR : wrong attribute in triangle");
	}
	if (std::string(xml->getAttributeValue("A")) == std::string(""))
		throw std::runtime_error(
			"XML PARSE ERROR : triangle should have a [A] attribute");
	if (std::string(xml->getAttributeValue("B")) == std::string(""))
		throw std::runtime_error(
			"XML PARSE ERROR : triangle should have a [B] attribute");
	if (std::string(xml->getAttributeValue("C")) == std::string(""))
		throw std::runtime_error(
			"XML PARSE ERROR : triangle should have a [C] attribute");
	vec.X = (unsigned short)xml->getAttributeValueAsInt("A");
	vec.Y = (unsigned short)xml->getAttributeValueAsInt("B");
	vec.Z = (unsigned short)xml->getAttributeValueAsInt("C");
	return vec;
}

irr::video::SColor xml_misc::parseColor(irr::io::IXMLReaderUTF8* xml) {
	if (xml->getAttributeCount() != 3)
		throw std::runtime_error(
			"XML PARSE ERROR : color should have 3 attributes");
	irr::video::SColor col;
	for (unsigned int i = 0; i < xml->getAttributeCount(); ++i) {
		std::string attrib = xml->getAttributeName(i);
		if (attrib == std::string("Red")) continue;
		if (attrib == std::string("Green")) continue;
		if (attrib == std::string("Blue")) continue;
		throw std::runtime_error(
			"XML PARSE ERROR : wrong attribute in color");
	}
	if (std::string(xml->getAttributeValue("Red")) == std::string(""))
		throw std::runtime_error(
			"XML PARSE ERROR : color should have a [Red] attribute");
	if (std::string(xml->getAttributeValue("Green")) == std::string(""))
		throw std::runtime_error(
			"XML PARSE ERROR : color should have a [Green] attribute");
	if (std::string(xml->getAttributeValue("Blue")) == std::string(""))
		throw std::runtime_error(
			"XML PARSE ERROR : color should have a [Blue] attribute");
	col.setRed((unsigned int)(xml->getAttributeValueAsFloat("Red") * 255.0f));
	col.setGreen(
		(unsigned int)(xml->getAttributeValueAsFloat("Green") * 255.0f));
	col.setBlue((unsigned int)(xml->getAttributeValueAsFloat("Blue") * 255.0f));
	return col;
}

std::pair<std::string, std::string> xml_misc::parseItem(
	irr::io::IXMLReaderUTF8* xml) 
{
	if (xml->getAttributeCount() != 2)
		throw std::runtime_error(
			"XML PARSE ERROR : item attribute can only have 2 parameters");
	for (unsigned int i = 0; i < xml->getAttributeCount(); ++i) {
		std::string attrib = xml->getAttributeName(i);
		if (attrib == std::string("name")) continue;
		if (attrib == std::string("value")) continue;
		throw std::runtime_error(
			"XML PARSE ERROR : wrong attribute in item");
	}
	if (std::string(xml->getAttributeValue("name")) == std::string(""))
		throw std::runtime_error(
			"XML PARSE ERROR : item should have a [name] attribute");
	if (std::string(xml->getAttributeValue("value")) == std::string(""))
		throw std::runtime_error(
			"XML PARSE ERROR : item should have a [value] attribute");
	return std::make_pair<std::string, std::string>(
		std::string(xml->getAttributeValue("name")),
		std::string(xml->getAttributeValue("value")));
}

std::string xml_misc::parseLocation(
	irr::io::IXMLReaderUTF8* xml, 
	const std::string&  tag_name) 
{
	if (xml->getAttributeCount() > 0) {
		std::ostringstream oss("");
		oss << "XML PARSE ERROR : there is no attributes in ";
		oss << tag_name;
		throw std::runtime_error(oss.str().c_str());
	}
	do {
		if (xml->getNodeType() == irr::io::EXN_ELEMENT_END) {
			std::ostringstream oss("");
			oss << "XML PARSE ERROR : this node has no ";
			oss << tag_name;
			throw std::runtime_error(oss.str().c_str());
		}
		if (xml->getNodeType() == irr::io::EXN_TEXT) {
			std::string location = xml->getNodeData();
			if (!location.size()) {
				std::ostringstream oss("");
				oss << "XML PARSE ERROR : ";
				oss << tag_name;
				oss << " cannot be empty";
				throw std::runtime_error(oss.str().c_str());
			}
			return location;
		}
	} while (xml->read());
	std::ostringstream oss ("");
	oss << "XML PARSE ERROR : unexpected end of file in ";
	oss << tag_name;
	throw std::runtime_error(oss.str().c_str());
}

irr::video::S3DVertex xml_misc::parseVertex(irr::io::IXMLReaderUTF8* xml) {
	if (xml->getAttributeCount() != 0)
		throw std::runtime_error(
			"XML PARSE ERROR : vertex should not have attributes");
	irr::video::S3DVertex vertex;
	bool is_pos = false;
	bool is_normal = false;
	bool is_color = false;
	bool is_tcoords = false;
	while (xml->read()) {
		if (xml->getNodeType() == irr::io::EXN_ELEMENT) {
			std::string node_name = xml->getNodeName();
			if (node_name == std::string("position")) {
				if (is_pos)	throw std::runtime_error(
					"XML PARSER ERROR : only 1 position allowed in vertex");
				is_pos = true;
				vertex.Pos = xml_misc::parseVector3d(xml);
			}
			if (node_name == std::string("normal")) {
				if (is_normal) throw std::runtime_error(
					"XML PARSER ERROR : only 1 normal allowed in vertex");
				is_normal = true;
				vertex.Normal = xml_misc::parseVector3d(xml);
			}
			if (node_name == std::string("color")) {
				if (is_color) throw std::runtime_error(
					"XML PARSER ERROR : only 1 color allowed in vertex");
				is_color = true;
				vertex.Color = xml_misc::parseColor(xml);
			}
			if (node_name == std::string("texture")) {
				if (is_tcoords) throw std::runtime_error(
					"XML PARSER ERROR : only 1 texture allowed in vertex");
				is_tcoords = true;
				vertex.TCoords = xml_misc::parseVector2d(xml);
			}			
		}
		if ((xml->getNodeType() == irr::io::EXN_ELEMENT_END) &&
			(xml->getNodeName() == std::string("vertex"))) 
		{ 
			if (is_pos && is_normal && is_color && is_tcoords) {
				return vertex;
			} else {
				std::stringstream ss("");
				ss << "XML PARSER ERROR : element missing in vertex :";
				if (is_pos) ss << " [position]";
				if (is_normal) ss << " [normal]";
				if (is_color) ss << " [color]";
				if (is_tcoords) ss << " [texture]";
				throw std::runtime_error(ss.str().c_str());
			}

		}
		// TODO should probably throw here too
	}
	throw std::runtime_error(
		"XML PARSE ERROR : Unexpected end of file in vertex");
}

plant xml_misc::parsePlant(irr::io::IXMLReaderUTF8* xml) {
	if (xml->getAttributeCount() != 3)
		throw std::runtime_error(
			"XML PARSE ERROR : plant must have 3 attributes");
	for (unsigned int i = 0; i < xml->getAttributeCount(); ++i) {
		std::string attrib = xml->getAttributeName(i);
		if (attrib == std::string("owner")) continue;
		if (attrib == std::string("energy")) continue;
		if (attrib == std::string("hit")) continue;
		throw std::runtime_error(
			"XML PARSE ERROR : Wrong attributes in plant");
	}
	int owner = xml->getAttributeValueAsInt("owner");
	float energy = xml->getAttributeValueAsFloat("energy");
	bool hit = 
		(std::string(xml->getAttributeValue("hit")) == std::string("true")) ? 
		true : 
		false;
	bool is_position = false;
	irr::core::vector3df position;
	while (xml->read()) {
		if (xml->getNodeType() == irr::io::EXN_ELEMENT) {
			std::string nodeName = xml->getNodeName();
			if (nodeName == std::string("position")) {
				if (is_position) throw std::runtime_error(
					"XML PARSE ERROR : only one position per plant");
				is_position = true;
				position = parseVector3d(xml);
			}
		}
		if ((xml->getNodeType() == irr::io::EXN_ELEMENT_END) &&
			(xml->getNodeName() == std::string("position"))) 
		{
			if (is_position) {
				plant pl;
				pl.m_energy = energy;
				pl.m_position = position;
				pl.m_hit = hit;
				pl.m_player_id = owner;
				return pl;
			} else {
				throw std::runtime_error(
					"XML PARSE ERROR : missing position in plant");
			}
		}
	}
	throw std::runtime_error(
		"XML PARSE ERROR : Unexpected end of file in plant");
}

std::string xml_misc::parseImageUp(irr::io::IXMLReaderUTF8* xml) {
	return parseLocation(xml, std::string("image-up"));
}

std::string xml_misc::parseImageDown(irr::io::IXMLReaderUTF8* xml) {
	return parseLocation(xml, std::string("image-down"));
}

std::string xml_misc::parseImageFlyby(irr::io::IXMLReaderUTF8* xml) {
	return parseLocation(xml, std::string("image-flyby"));
}

std::string xml_misc::parseImageUpFlyby(irr::io::IXMLReaderUTF8* xml) {
	return parseLocation(xml, std::string("image-up-flyby"));
}

std::string xml_misc::parseImageDownFlyby(irr::io::IXMLReaderUTF8* xml) {
	return parseLocation(xml, std::string("image-down-flyby"));
}

void xml_misc::saveVertex(
	std::ostream& os, 
	const irr::video::S3DVertex& vert,
	unsigned int level)
{
	for (unsigned int i = 0; i < level; ++i) os << "\t";
	os << "<vertex>" << std::endl;
	savePosition(os, vert.Pos, level + 1);
	saveNormal(os, vert.Normal, level + 1);
	saveColor(os, vert.Color, level + 1);
	saveTexture(os, vert.TCoords, level + 1);
	for (unsigned int i = 0; i < level; ++i) os << "\t";
	os << "</vertex>" << std::endl;
}

void xml_misc::savePosition(
	std::ostream& os,
	const irr::core::vector3df& vec,
	unsigned int level)
{
	for (unsigned int i = 0; i < level; ++i) os << "\t";
	os << "<position";
	os << " X=\"" << vec.X << "\"";
	os << " Y=\"" << vec.Y << "\"";
	os << " Z=\"" << vec.Z << "\"";
	os << "/>" << std::endl;
}

void xml_misc::saveNormal(
	std::ostream& os,
	const irr::core::vector3df& vec,
	unsigned int level)
{
	for (unsigned int i = 0; i < level; ++i) os << "\t";
	os << "<normal";
	os << " X=\"" << vec.X << "\"";
	os << " Y=\"" << vec.Y << "\"";
	os << " Z=\"" << vec.Z << "\"";
	os << "/>" << std::endl;
}

void xml_misc::saveTexture(
	std::ostream& os,
	const irr::core::vector2df& vec,
	unsigned int level)
{
	for (unsigned int i = 0; i < level; ++i) os << "\t";
	os << "<texture";
	os << " X=\"" << vec.X << "\"";
	os << " Y=\"" << vec.Y << "\"";
	os << "/>" << std::endl;
}

void xml_misc::saveColor(
	std::ostream& os,
	const irr::video::SColor& col,
	unsigned int level,
	const std::string& post)
{
	for (unsigned int i = 0; i < level; ++i) os << "\t";
	os << "<color" << post;
	os << " Red=\"" << ((float)(col.getRed()) / 255.0f) << "\"";
	os << " Green=\"" << ((float)(col.getGreen()) / 255.0f) << "\"";
	os << " Blue=\"" << ((float)(col.getBlue()) / 255.0f) << "\"";
	os << "/>" << std::endl;
}

void xml_misc::saveTriangle(
	std::ostream& os,
	unsigned short A,
	unsigned short B,
	unsigned short C,
	unsigned int level)
{
	for (unsigned int i = 0; i < level; ++i) os << "\t";
	os << "<triangle";
	os << " A=\"" << A << "\"";
	os << " B=\"" << B << "\"";
	os << " C=\"" << C << "\"";
	os << "/>" << std::endl;
}