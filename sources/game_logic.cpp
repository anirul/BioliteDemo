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
	
#include "game_logic.h"
#include "sound.h"

game_logic::game_logic(
	float update_freq,
	std::vector<float> player_energy,
	float plant_speed,
	float plant_cost,
	float plant_income,
	float fruit_cost,
	float destroy_multiply) 
	:	m_timer(NULL),
		m_last_called(0.0f),
		m_update_freq(update_freq),
		m_player_energy(player_energy),
		m_plant_speed(plant_speed),
		m_plant_cost(plant_cost),
		m_plant_income(plant_income),
		m_fruit_cost(fruit_cost),
		m_destroy_multiply(destroy_multiply),
		m_ai_easy(NULL),
		m_ai_medium(NULL),
		m_ai_hard(NULL)
{
	parameter_set::instance()->setValue(
		std::string("biolite.plant.cost"),
		m_plant_cost);
	parameter_set::instance()->setValue(
		std::string("biolite.fruit.cost"),
		m_fruit_cost);
	m_min_dist = (float)parameter_set::instance()->getValueDouble(
		"biolite.logic.min-distance");
	m_max_dist = (float)parameter_set::instance()->getValueDouble(
		"biolite.logic.max-distance");
	m_nrj_1 = (float)parameter_set::instance()->getValueDouble(
		"biolite.logic.grow1");
	m_nrj_2 = (float)parameter_set::instance()->getValueDouble(
		"biolite.logic.grow2");
	m_plant_store = (float)parameter_set::instance()->getValueDouble(
		"biolite.logic.plant-store");
	{
		std::stringstream ss("");
		ss << (int)(0.1 * m_destroy_multiply);
		ss << "-";
		ss << (int)(m_plant_store * m_destroy_multiply);
		parameter_set::instance()->setValue(
			std::string("biolite.destroy.cost"),
			ss.str());
	}
	for (int i = 0; i < MAX_CLIENT; ++i)
		m_player_plant_map.insert(std::make_pair(i, 0));
	for (int i = 0; i < MAX_CLIENT; ++i) 
		m_player_fruit_map.insert(std::make_pair(i, 0));
	m_plant_flyby.m_player_id = 0;
	m_plant_flyby.m_energy = 10.0;
	m_plant_flyby.m_update = true;
	m_plant_flyby.m_plant_t = plant::leaf;
	parameter_set::instance()->addListener(this);
}

game_logic::~game_logic() {
	m_plant_list.clear();
	if (m_ai_easy) delete m_ai_easy;
	if (m_ai_medium) delete m_ai_medium;
	if (m_ai_hard) delete m_ai_hard;
	parameter_set::instance()->removeListener(this);
}

void game_logic::operator()(
	const std::string& name, 
	const std::string& value) 
{
	// TODO react on select building type
	// TODO react in case a end on case is met
}

void game_logic::tick() {
	tickAi();
	tickClick();
	tickPlant();
	tickDisplay();
	tickEnd();
}

void game_logic::tickAi() {
	std::string ai_difficulty =
		parameter_set::instance()->getValue("biolite.ai.difficulty");
	std::string ai_number =
		parameter_set::instance()->getValue("biolite.ai.number");
	std::stringstream ss(ai_number);
	int number;
	ss >> number;
	if (number < 0) throw std::runtime_error(
		"ERROR : AI number cannot be < 0");
	if (number > MAX_CLIENT - 1) throw std::runtime_error(
		"ERROR : too many AIs");
	for (int i = 0; i < number; ++i) {
		if (ai_difficulty == std::string("easy")) {
			if (m_ai_easy) {
				m_ai_easy->tick(i + 1);
			}
		}
		if (ai_difficulty == std::string("medium")) {
			if (m_ai_medium) {
				m_ai_medium->tick(i + 1);
				// help the AI cheeting a little bit
				if (m_player_energy.at(i + 1))
					m_player_energy.at(i + 1) += 0.5;
			}
		}
		if (ai_difficulty == std::string("hard")) {
			if (m_ai_hard) {
				m_ai_hard->tick(i + 1);
				// help the AI cheeting a little bit
				if (m_player_energy.at(i + 1))
					m_player_energy.at(i + 1) += 5;
			}
		}
	}
}

void game_logic::tickClick() {
	// check users (AI + player) clicks
	std::list<click_desc>::iterator ite;
	for (ite = m_player_click_list.begin(); 
		ite != m_player_click_list.end(); 
		++ite)
	{
		switch (ite->m_click_t) {
		case click_desc::ct_destroy :
			clickDestroy((*ite));
			continue;
		case click_desc::ct_fruit_tree :
			clickFruit((*ite));
			continue;
		case click_desc::ct_leaf_tree :
		default :
			clickLeaf((*ite));
			continue;
		}
	}
	m_player_click_list.clear();
}

void game_logic::clickDestroy(const click_desc& cd) {
	{ // check distance
		if (cd.m_position.getLength() <= 1.0f) {
			if (cd.m_player_id == 0)
				sound::instance()->play("error");
			return;
		}
		std::list<plant>::iterator ite;
		irr::core::vector3df tp = cd.m_position;
		float closest_mine = 1.0f;
		int current_count = 0;
		// check distance
		for (ite = m_plant_list.begin(); ite != m_plant_list.end(); ++ite) {
			irr::core::vector3df pos = ite->m_position;
			float distance = (tp - pos).getLength();
			if (ite->m_player_id == cd.m_player_id)
				if (closest_mine > distance)
					closest_mine = distance;
		}
		// check if the current player has any plant nearby
		// check also that this is not a player plant
		if ((closest_mine > m_max_dist) || (closest_mine < m_min_dist)) {
			if (cd.m_player_id == 0)
				sound::instance()->play("error");
			return;
		}
	}
	{ // destroy
		std::list<plant>::iterator ite;
		for (ite = m_plant_list.begin(); ite != m_plant_list.end(); ++ite) {
			if (ite->m_player_id == cd.m_player_id) continue;
			irr::core::vector3df distance = ite->m_position - cd.m_position;
			// check distance
			if (distance.getLength() < m_min_dist) {
				// check energy
				if (m_player_energy.at(cd.m_player_id) > 
					ite->m_energy * m_destroy_multiply) 
				{
					// pay
					m_player_energy.at(cd.m_player_id) -=
						ite->m_energy * m_destroy_multiply;
					// this tree was hit
					ite->m_hit = true;
					// if this was a mamatree then take over
					if (ite->m_plant_mesh_t == plant::mama_tree)
						takeOverPlayer(ite->m_player_id, cd.m_player_id);
					if (cd.m_player_id == 0)
						sound::instance()->play("planting");
					if (ite->m_player_id == 0)
						sound::instance()->play("wrong");
					return;
				} else {
					if (cd.m_player_id == 0)
						sound::instance()->play("wrong");
				}
			}
		}
	}
}

void game_logic::tickEnd() {
	int count_player = 0;
	int count_enemy = 0;
	std::list<plant>::iterator ite;
	for (ite = m_plant_list.begin(); ite != m_plant_list.end(); ++ite) {
		if (ite->m_player_id == 0) {
			count_player++;
		} else {
			count_enemy++;
		}
	}
	const float nrj_player = m_player_energy.at(0);
	float nrj_enemy = 0.0f;
	for (int i = 1; i < MAX_CLIENT; ++i) {
		nrj_enemy += m_player_energy.at(i);
	}
	if (count_player == 0 && nrj_player == 0.0f)
		parameter_set::instance()->setValue(
											std::string("biolite.game.end"), 
											std::string("defeat"));
	if (static_cast<float> (count_enemy) * nrj_enemy == 0.0f)
		parameter_set::instance()->setValue(
											std::string("biolite.game.end"),
											std::string("victory"));
}

void game_logic::clickFruit(const click_desc& cd) {
	// check if player can buy a plant
	if (m_player_energy.at(cd.m_player_id) >= m_fruit_cost) {
		plant p;
		p.m_plant_t = plant::fruit;
		p.m_player_id = cd.m_player_id;
		p.m_plant_mesh_t = plant::null;
		p.m_energy = 0.1f;
		p.m_hit = false;
		p.m_position = cd.m_position;
		if (addPlant(p)) {
			// plant added so pay!
			m_player_energy.at(cd.m_player_id) -= m_fruit_cost;
			if (cd.m_player_id == 0)
				sound::instance()->play("planting");
		} else {
			if (cd.m_player_id == 0)
				sound::instance()->play("wrong");
		}
	} else {
		if (cd.m_player_id == 0)
			sound::instance()->play("error");
	}

}
		
void game_logic::clickLeaf(const click_desc& cd) {
	// check if player can buy a plant
	if (m_player_energy.at(cd.m_player_id) >= m_plant_cost) {
		plant p;
		p.m_plant_t = plant::leaf;
		p.m_player_id = cd.m_player_id;
		p.m_plant_mesh_t = plant::null;
		p.m_energy = 0.1f;
		p.m_hit = false;
		p.m_position = cd.m_position;
		if (addPlant(p)) {
			// plant added so pay!
			m_player_energy.at(cd.m_player_id) -= m_plant_cost;
			if (cd.m_player_id == 0)
				sound::instance()->play("planting");
		} else {
			if (cd.m_player_id == 0)
				sound::instance()->play("wrong");
		}
	} else {
		if (cd.m_player_id == 0)
			sound::instance()->play("error");
	}
}

void game_logic::tickPlant() {
	// update all the plants
	std::list<plant>::iterator ite;
	std::list<std::list<plant>::iterator> remove_list;
	for (ite = m_plant_list.begin(); ite != m_plant_list.end(); ++ite) {
		plant& p = (*ite);
		irr::core::vector3df n = p.m_position;
		n.normalize();
		irr::core::vector3df sun = m_sun_pos;
		sun.normalize();
		float exposure = n.dotProduct(sun);
		int nb_plant = m_player_plant_map[p.m_player_id];
		int nb_fruit = m_player_fruit_map[p.m_player_id];
		// skip ghost plant
		if ((p.m_plant_mesh_t == plant::ghost_red) || 
			(p.m_plant_mesh_t == plant::ghost_green))
			continue;
		// dying plant
		if (p.m_hit)
			p.m_energy -= 0.1f;
		// remove dead plants
		if (p.m_energy <= 0.0f) {
			p.remove();
			remove_list.push_back(ite);
			continue;
		}
		// first plant of current player (mama_tree)
		if (nb_plant == 0) {
			p.m_energy = m_plant_store;
			p.add(plant::mama_tree, m_parent, m_mgr);
			continue;
		}
		if ((!p.m_hit) && (p.m_plant_mesh_t == plant::mama_tree)) {
			p.m_energy = m_plant_store;
			p.add(plant::mama_tree, m_parent, m_mgr);
			// mama tree also increase energy
			float& player_nrj = m_player_energy.at(p.m_player_id);
			if (exposure > 0.0f) player_nrj += exposure * m_plant_income;
			continue;
		}
		if ((exposure > 0.0f) && (!p.m_hit)) // grow during day
			p.m_energy += exposure * m_plant_speed;
		if ((p.m_energy <= m_nrj_1) && (p.m_energy > 0.0f)) {
			if (p.m_hit)
				p.add(plant::plant_die_1, m_parent, m_mgr);
			else
				p.add(plant::plant_grow_1, m_parent, m_mgr);
		}
		if ((p.m_energy > m_nrj_1) && (p.m_energy < m_nrj_2)) {
			if (p.m_hit)
				p.add(plant::plant_die_2, m_parent, m_mgr);
			else
				p.add(plant::plant_grow_2, m_parent, m_mgr);
		}
		// in case max size
		if (p.m_energy >= m_nrj_2) {
			p.m_energy = m_nrj_2;
			if (p.m_hit) {
				p.add(plant::plant_die_3, m_parent, m_mgr);
				continue;
			}
			if (p.m_plant_t == plant::fruit) {
				p.add(plant::fruit_tree, m_parent, m_mgr);
				continue;
			}
			p.add(plant::plant_grow_3, m_parent, m_mgr);
			// income time!
			float& player_nrj = m_player_energy.at(p.m_player_id);
			float calc_income = ((player_nrj + 10) / 500) * m_plant_income;
			if (exposure > 0.0f) player_nrj += exposure * calc_income;
		}
	}
	std::list<std::list<plant>::iterator>::iterator rmite;
	for (rmite = remove_list.begin(); rmite != remove_list.end(); ++rmite) {
		m_plant_list.erase((*rmite));
	}
}

void game_logic::tickDisplay() {
	// update display
	for (unsigned int i = 0; i < MAX_CLIENT; ++i) {
		{ // energy per player
			std::string name;
			std::string value;
			{
				float& energy = m_player_energy.at(i);
				int nb_fruit = m_player_fruit_map[i];
				if (energy > m_plant_store * (nb_fruit + 1))
					energy = m_plant_store * (nb_fruit + 1);
				if (energy < 0.0f) 
					energy = 0.0f;
				std::stringstream ss("");
				ss << (int)energy;
				value = ss.str();
			}
			{
				std::stringstream ss("");
				ss << "biolite.player";
				ss << i;
				ss << ".energy";
				name = ss.str();
			}
			parameter_set::instance()->setValue(name, value);
		}
		{ // plant per player
			std::string name;
			std::string value;
			{
				int nb_plant = 0;
				int nb_fruit = 0;
				std::list<plant>::iterator ite;
				for (ite = m_plant_list.begin(); 
					ite != m_plant_list.end(); 
					++ite) 
				{
					if (ite->m_player_id == i) {
						nb_plant++;
						if (ite->m_plant_t == plant::fruit) {
							nb_fruit++;
						}
					}
				}
				std::stringstream ss("");
				ss << nb_plant;
				m_player_plant_map[i] = nb_plant;
				m_player_fruit_map[i] = nb_fruit;
				value = ss.str();
			}
			{
				std::stringstream ss("");
				ss << "biolite.player";
				ss << i;
				ss << ".plant.size";
				name = ss.str();
			}
			parameter_set::instance()->setValue(name, value);
		}
	}
}

void game_logic::render(game* pg, irr::IrrlichtDevice* pdevice) {
	// init AIs
	if (!m_ai_easy) 
		m_ai_easy = new easy_ai(this, pg->m_pworld);
	if (!m_ai_medium) 
		m_ai_medium = new medium_ai(this, pg->m_pworld);
	if (!m_ai_hard) 
		m_ai_hard = new hard_ai(this, pg->m_pworld);
	// update sun position
	m_sun_pos = pg->m_pworld->getSunPosition();
	// store parent and manager to be able to render
	m_parent = pg->m_pworld->m_planet_node;
	m_mgr = pdevice->getSceneManager();
	// call periodic logic
	const float wait_time = 1.0f / m_update_freq;
	if (!m_timer) m_timer = pdevice->getTimer();
	float now = (float)(m_timer->getTime()) / 1000.0f;
	if ((now - m_last_called) > wait_time) {
		m_last_called = now;
		tick();
	}
}

bool game_logic::planetClick(const click_desc& cd)
{
	if (cd.m_position.getLength()) {
		m_player_click_list.push_back(cd);
		return true;
	}
	return false;
}

bool game_logic::planetFlyby(const irr::core::vector3df& hit) {
	if ((hit.getLength() <= 0.0f) || (hit.getLength() > 2.0f)){
		m_plant_flyby.remove();
		return false;
	}
	m_plant_flyby.m_position = hit;
	if (checkPlant(hit)) {
		m_plant_flyby.add(plant::ghost_green, m_parent, m_mgr);
	} else {
		m_plant_flyby.add(plant::ghost_red, m_parent, m_mgr);
	}
	return true;
}

bool game_logic::checkPlant(const irr::core::vector3df& hit, int player_id) {
	float closest = 1.0f;
	float closest_mine = 1.0f;
	int current_count = 0;
	// check distance
	std::list<plant>::iterator ite;
	for (ite = m_plant_list.begin(); ite != m_plant_list.end(); ++ite) {
		irr::core::vector3df pos = ite->m_position;
		float distance = (hit - pos).getLength();
		if (distance < closest) {
			closest = distance;
			if (ite->m_player_id == player_id)
				if ((ite->m_plant_mesh_t == plant::mama_tree) ||
					(ite->m_plant_mesh_t == plant::fruit_tree) ||
					(ite->m_plant_mesh_t == plant::plant_grow_3))
					closest_mine = distance;
		}
		if (ite->m_player_id == player_id) current_count++;
	}
	// check if the current player has any plant
	if (current_count)
		if (closest_mine > m_max_dist) 
			return false;
	if (closest < m_min_dist) return false;
	return true;
}

bool game_logic::addPlant(const plant& pl) {
	if (pl.m_position.getLength() <= 1.0f)
		return false;
	irr::core::vector3df tp = pl.m_position;
	if (!checkPlant(tp, pl.m_player_id)) return false;
	m_plant_list.push_back(pl);
	return true;
}

void game_logic::setPlanetName(const std::string& file) {
	parameter_set::instance()->setValue("biolite.planet.file", file);
}

void game_logic::takeOverPlayer(int player_lose, int player_win) {
	m_player_energy.at(player_win) =+ m_player_energy.at(player_lose);
	m_player_energy.at(player_lose) = 0;
	std::list<plant>::iterator ite;
	for (ite = m_plant_list.begin();
		ite != m_plant_list.end();
		++ite)
		if (ite->m_player_id == player_lose)
			ite->m_hit = true;
}