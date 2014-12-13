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
#include "xml_game_logic.h"

std::string xml_game_logic::parseGameLogic(irr::io::IXMLReaderUTF8* xml) {
	bool fault = true;
	std::string file("");
	if (xml->getAttributeCount() > 0)
		throw std::runtime_error(
								 "XML PARSE ERROR : there is no attributes in game");
	do {
		if (xml->getNodeType() == irr::io::EXN_ELEMENT_END)
			throw std::runtime_error("XML PARSE ERROR : this node has no text");
		if (xml->getNodeType() == irr::io::EXN_TEXT) {
			file = xml->getNodeData();
			if (!file.size())
				throw std::runtime_error(
										 "XML PARSE ERROR : game cannot be empty");
			fault = false;
			break;
		}
	} while (xml->read());
	if (fault)
		throw std::runtime_error(
								 "XML PARSE ERROR : unexpected end of file in game");
	return file;
}

xml_game_logic* xml_game_logic::parseGameLogic(
											   const std::string& file,
											   irr::io::IFileSystem* fs)
{
	// game XML loading
	irr::io::IXMLReaderUTF8* xml =
	fs->createXMLReaderUTF8(
							getPathOfMedia(file.c_str(), "xml").c_str());
	if (!xml) {
		std::ostringstream oss("");
		oss << "ERROR : [" << file << "] not found!";
		throw std::runtime_error(oss.str());
	}
	// attributes values
	float update_freq;
	std::vector<float> player_energy(4);
	float plant_speed;
	float dryad_life;
	float dryad_cost;
	float harvester_life;
	float harvester_cost;
	float damager_life;
	float damager_cost;
	float plant_income;
	float fruit_cost;
	float fruit_energy;
	std::string planet_name("");
	std::list<plant> plant_list;
	// parse the game file
	while (xml->read()) {
		if (xml->getNodeType() == irr::io::EXN_ELEMENT) {
			std::string elementName = xml->getNodeName();
			if (elementName == std::string("game")) {
				if (xml->getAttributeCount() != 15)
					throw std::runtime_error("XML PARSE ERROR : game must have 15 attributes");
				for (unsigned int i = 0; i < xml->getAttributeCount(); ++i) {
					std::string attrib = xml->getAttributeName(i);
					if (attrib == std::string("update-freq")) continue;
					if (attrib == std::string("player0-energy")) continue;
					if (attrib == std::string("player1-energy")) continue;
					if (attrib == std::string("player2-energy")) continue;
					if (attrib == std::string("player3-energy")) continue;
					if (attrib == std::string("plant-speed")) continue;
					if (attrib == std::string("plant-income")) continue;
					if (attrib == std::string("dryad-life")) continue;
					if (attrib == std::string("dryad-cost")) continue;
					if (attrib == std::string("harvester-life")) continue;
					if (attrib == std::string("harvester-cost")) continue;
					if (attrib == std::string("damager-life")) continue;
					if (attrib == std::string("damager-cost")) continue;
					if (attrib == std::string("fruit-cost")) continue;
					if (attrib == std::string("fruit-energy")) continue;
					{
						std::stringstream ss("");
						ss << "XML PARSER ERROR : unknown attribute in game : ";
						ss << attrib;
						throw std::runtime_error(ss.str());
					}
				}
				update_freq = xml->getAttributeValueAsFloat("update-freq");
				player_energy.at(0) =
				xml->getAttributeValueAsFloat("player0-energy");
				player_energy.at(1) =
				xml->getAttributeValueAsFloat("player1-energy");
				player_energy.at(2) =
				xml->getAttributeValueAsFloat("player2-energy");
				player_energy.at(3) =
				xml->getAttributeValueAsFloat("player3-energy");
				plant_speed = xml->getAttributeValueAsFloat("plant-speed");
				dryad_life = xml->getAttributeValueAsFloat("dryad-life");
				dryad_cost = xml->getAttributeValueAsFloat("dryad-cost");
				harvester_life = xml->getAttributeValueAsFloat("harvester-life");
				harvester_cost = xml->getAttributeValueAsFloat("harvester-cost");
				damager_life = xml->getAttributeValueAsFloat("damager-life");
				damager_cost = xml->getAttributeValueAsFloat("damager-cost");
				plant_income = xml->getAttributeValueAsFloat("plant-income");
				fruit_cost = xml->getAttributeValueAsFloat("fruit-cost");
				fruit_energy = xml->getAttributeValueAsFloat("fruit-energy");
			}
			// found a planet
			if (elementName == std::string("planet")) {
				if (planet_name != std::string("")) {
					planet_name = xml_planet::parsePlanet(xml);
				} else {
					throw std::runtime_error("XML PARSE ERROR : only one planet per game");
				}
			}
			if (elementName == std::string("plant"))
				plant_list.push_back(xml_misc::parsePlant(xml));
		}
		// check for closure of game
		if ((xml->getNodeType() == irr::io::EXN_ELEMENT_END) &&
			(std::string(xml->getNodeName()) == std::string("game")))
		{
			xml_game_logic* xgl = new xml_game_logic(update_freq,
													 player_energy,
													 plant_speed,
													 dryad_life,
													 dryad_cost,
													 harvester_life,
													 harvester_cost,
													 damager_life,
													 damager_cost,
													 plant_income,
													 fruit_cost,
													 fruit_energy);
			if (planet_name.size())
				xgl->setPlanetName(planet_name);
			else
				xgl->setPlanetName("<current>");
			std::list<plant>::iterator ite;
			for (ite = plant_list.begin(); ite != plant_list.end(); ++ite)
				xgl->addPlant((*ite));
			return xgl;
		}
	}
	return NULL;
}

xml_game_logic::xml_game_logic(float update_freq,
							   std::vector<float> player_energy,
							   float plant_speed,
							   float dryad_life,
							   float dryad_cost,
							   float harvester_life,
							   float harvester_cost,
							   float damager_life,
							   float damager_cost,
							   float plant_income,
							   float fruit_cost,
							   float fruit_energy) :
game_logic(update_freq,
		   player_energy,
		   plant_speed,
		   dryad_life,
		   dryad_cost,
		   harvester_life,
		   harvester_cost,
		   damager_life,
		   damager_cost,
		   plant_income,
		   fruit_cost,
		   fruit_energy) {}

xml_game_logic::~xml_game_logic() {}