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

#ifndef BIOLITE_GAME_LOGIC_HEADER_DEFINED
#define BIOLITE_GAME_LOGIC_HEADER_DEFINED

#include "parameter_set.h"
#include "plant.h"
#include "game.h"
#include "ai.h"

struct click_desc {
	enum click_type {
		ct_dryad,
		ct_harvester,
		ct_damager,
		ct_fetch,
		ct_first,
	} m_click_t;
	int m_player_id;
	irr::core::vector3df m_position;
};

class game_logic : public parameter_set_listener {
protected:
	unsigned int m_counter;
	irr::ITimer* m_timer;
	float m_last_called;
	std::list<plant> m_plant_list;
	plant m_plant_flyby;
	std::list<click_desc> m_player_click_list;
	std::map<int, int> m_player_plant_map;
	// parameter from contructor
	float m_update_freq;
	std::vector<float> m_player_energy;
	float m_plant_speed;
	float m_plant_income;
	float m_dryad_life;
	float m_dryad_cost;
	float m_harvester_life;
	float m_harvester_cost;
	float m_damager_life;
	float m_damager_cost;
	float m_fruit_energy;
	float m_min_dist;
	float m_max_dist;
	float m_nrj_1;
	float m_nrj_2;
	irr::core::vector3df m_sun_pos;
	irr::scene::ISceneNode* m_parent;
	irr::scene::ISceneManager* m_mgr;
	easy_ai* m_ai_easy;
	medium_ai* m_ai_medium;
	hard_ai* m_ai_hard;
public:
	// click actions
	void clickDryad(const click_desc& cd);
	void clickHarvester(const click_desc& cd);
	void clickDamager(const click_desc& cd);
	void clickFetch(const click_desc& cd);
	// tick
	void tickAi();
	void tickClick();
	void tickPlant();
	void tickDisplay();
	void tickEnd();
public:
	//! from parameter_set_listener
	virtual void operator()(const std::string& name, const std::string& value);
public:
	//! ctor dtor
	game_logic(
		float update_freq,
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
		float fruit_energy);
	virtual ~game_logic();
public:
	//! update game logic
	void tick();
	//! just to pass event from the game_state_xml_game or AI
	bool planetClick(const click_desc& cd);
	//! display a flyby ghost plant
	bool planetFlyby(const irr::core::vector3df& hit);
	//! render the plants on the planet
	void render(game* pg, irr::IrrlichtDevice* pdevice);
	//! check plant location (mostly distance)
	bool checkPlant(const irr::core::vector3df& hit, int player_id = 0);
	//! add a plant to the plant list
	bool addPlant(const plant& pl);
	//! associated planet
	void setPlanetName(const std::string& file);
	//! get the plant list
	const std::list<plant>&	getPlantList() const { return m_plant_list; }
	//! get the sun last known position
	const irr::core::vector3df& getSunPosition() const { 
		return m_sun_pos; 
	}
	//! get the energy of a player
	float getPlayerEnergy(int player) const { 
		return m_player_energy.at(player); 
	}
	void takeOverPlayer(int player_lose, int player_win);
};

#endif // BIOLITE_GAME_LOGIC_HEADER_DEFINED