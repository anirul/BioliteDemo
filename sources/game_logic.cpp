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
	float dryad_life,
	float dryad_cost,
	float harvester_life,
	float harvester_cost,
	float damager_life,
	float damager_cost,
	float plant_income,
	float fruit_cost,
	float fruit_energy)
	:	m_counter(0),
		m_timer(NULL),
		m_last_called(0.0f),
		m_update_freq(update_freq),
		m_player_energy(player_energy),
		m_plant_speed(plant_speed),
		m_plant_income(plant_income),
		m_dryad_life(dryad_life),
		m_dryad_cost(dryad_cost),
		m_harvester_life(harvester_life),
		m_harvester_cost(harvester_cost),
		m_damager_life(damager_life),
		m_damager_cost(damager_cost),
		m_fruit_energy(fruit_energy),
		m_ai_easy(NULL),
		m_ai_medium(NULL),
		m_ai_hard(NULL)
{
	parameter_set::instance()->setValue("biolite.dryad.cost", m_dryad_cost);
	parameter_set::instance()->setValue("biolite.harvester.cost", m_harvester_cost);
	parameter_set::instance()->setValue("biolite.damager.cost", m_damager_cost);
	m_min_dist = (float)parameter_set::instance()->getValueDouble(
		"biolite.logic.min-distance");
	m_max_dist = (float)parameter_set::instance()->getValueDouble(
		"biolite.logic.max-distance");
	m_nrj_1 = (float)parameter_set::instance()->getValueDouble(
		"biolite.logic.grow1");
	m_nrj_2 = (float)parameter_set::instance()->getValueDouble(
		"biolite.logic.grow2");
	for (int i = 0; i < MAX_CLIENT; ++i)
		m_player_plant_map.insert(std::make_pair(i, 0));
	m_plant_flyby.m_player_id = 0;
	m_plant_flyby.m_energy = 10.0;
	m_plant_flyby.m_update = true;
	m_plant_flyby.m_plant_t = plant::harvester;
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
					m_player_energy.at(i + 1) += 0.01;
			}
		}
		if (ai_difficulty == std::string("hard")) {
			if (m_ai_hard) {
				m_ai_hard->tick(i + 1);
				// help the AI cheeting a lot
				if (m_player_energy.at(i + 1))
					m_player_energy.at(i + 1) += 0.1;
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
			case click_desc::ct_dryad :
				clickDryad((*ite));
				continue;
			case click_desc::ct_harvester :
				clickHarvester((*ite));
				continue;
			case click_desc::ct_damager :
				clickDamager((*ite));
				continue;
			case click_desc::ct_fetch :
				clickFetch((*ite));
				continue;
			default:
				std::cerr << "unknown click type" << std::endl;
				continue;
		}
	}
	m_player_click_list.clear();
}

// plant a dryad into the world!
void game_logic::clickDryad(const click_desc& cd) {
	// check if player can buy a plant
	if (m_player_energy.at(cd.m_player_id) >= m_dryad_cost) {
		plant p;
		p.m_plant_t = plant::dryad;
		p.m_player_id = cd.m_player_id;
		p.m_plant_mesh_t = plant::null;
		p.m_energy = 0.1f;
		p.m_hit = false;
		p.m_position = cd.m_position;
		if (addPlant(p)) {
			// plant added so pay!
			m_player_energy.at(cd.m_player_id) -= m_dryad_cost;
			if (cd.m_player_id == 0) {
				sound::instance()->play("planting");
			}
		} else {
			if (cd.m_player_id == 0)
				sound::instance()->play("wrong");
		}
	} else {
		if (cd.m_player_id == 0)
			sound::instance()->play("error");
	}
	if (cd.m_player_id == 0)
		parameter_set::instance()->setValue("biolite.action.type", "fetch");
}

void game_logic::clickHarvester(const click_desc& cd) {
	// check if player can buy a plant
	if (m_player_energy.at(cd.m_player_id) >= m_harvester_cost) {
		plant p;
		p.m_plant_t = plant::harvester;
		p.m_player_id = cd.m_player_id;
		p.m_plant_mesh_t = plant::null;
		p.m_energy = 0.1f;
		p.m_hit = false;
		p.m_position = cd.m_position;
		if (addPlant(p)) {
			// plant added so pay!
			m_player_energy.at(cd.m_player_id) -= m_dryad_cost;
			if (cd.m_player_id == 0) {
				sound::instance()->play("planting");
			}
		} else {
			if (cd.m_player_id == 0)
				sound::instance()->play("wrong");
		}
	} else {
		if (cd.m_player_id == 0)
			sound::instance()->play("error");
	}
	if (cd.m_player_id == 0)
		parameter_set::instance()->setValue("biolite.action.type", "fetch");
}

void game_logic::clickDamager(const click_desc& cd) {
	// check if player can buy a plant
	if (m_player_energy.at(cd.m_player_id) >= m_damager_cost) {
		plant p;
		p.m_plant_t = plant::damager;
		p.m_player_id = cd.m_player_id;
		p.m_plant_mesh_t = plant::null;
		p.m_energy = 0.1f;
		p.m_hit = false;
		p.m_position = cd.m_position;
		if (addPlant(p)) {
			// plant added so pay!
			m_player_energy.at(cd.m_player_id) -= m_damager_cost;
			if (cd.m_player_id == 0) {
				sound::instance()->play("planting");
			}
		} else {
			if (cd.m_player_id == 0)
				sound::instance()->play("wrong");
		}
	} else {
		if (cd.m_player_id == 0)
			sound::instance()->play("error");
	}
	if (cd.m_player_id == 0)
		parameter_set::instance()->setValue("biolite.action.type", "fetch");
}

void game_logic::clickFetch(const click_desc& cd) {
	{ // check distance
		if (cd.m_position.getLength() <= 1.0f) {
			if (cd.m_player_id == 0)
				sound::instance()->play("error");
			return;
		}
		std::list<plant>::iterator ite;
		irr::core::vector3df tp = cd.m_position;
		float closest_mine = 1.0f;
		// check distance
		for (ite = m_plant_list.begin(); ite != m_plant_list.end(); ++ite) {
			irr::core::vector3df pos = ite->m_position;
			float distance = (tp - pos).getLength();
			if (ite->m_player_id == cd.m_player_id)
				if (closest_mine > distance)
					closest_mine = distance;
		}
		// check if the current player has any plant nearby
		// check also that this is a player plant
		if (closest_mine > m_min_dist) {
			if (cd.m_player_id == 0)
				sound::instance()->play("error");
			return;
		}
	}
	{ // gather
		for (auto& p : m_plant_list) {
			// can only gather on own plants
			if (p.m_player_id != cd.m_player_id) continue;
			if (p.m_plant_t != plant::harvester) continue;
			irr::core::vector3df distance = p.m_position - cd.m_position;
			// check distance
			if (distance.getLength() < m_min_dist) {
				// check if plant can be harvested
				if (!p.m_fruit) continue;
				p.m_fruit = false;
				p.m_energy = m_nrj_1;
				m_player_energy.at(cd.m_player_id) += m_fruit_energy;
			}
		}
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
			p.m_energy = m_dryad_life;
			p.add(plant::mama_tree, m_parent, m_mgr);
			continue;
		}
		if ((!p.m_hit) && (p.m_plant_mesh_t == plant::mama_tree)) {
			p.m_energy = m_dryad_life;
			p.add(plant::mama_tree, m_parent, m_mgr);
			// mama tree also increase energy
			m_player_energy.at(p.m_player_id) += 0.01;
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
			if (p.m_hit) {
				p.add(plant::plant_die_3, m_parent, m_mgr);
				continue;
			}
			switch (p.m_plant_t) {
				case plant::harvester:
				{
					float factor = log(m_player_plant_map[p.m_player_id]);
					if ((p.m_plant_t == plant::harvester) &&
						((p.m_energy >= m_nrj_2 * factor) ||
						p.m_fruit))
					{
						p.m_fruit = true;
						p.add(plant::fruit_tree, m_parent, m_mgr);
					}
					break;
				}
				case plant::dryad:
				{
					p.m_energy = m_nrj_2;
					p.add(plant::mama_tree, m_parent, m_mgr);
					break;
				}
				case plant::damager:
				{
					p.m_energy = m_nrj_2;
					p.add(plant::plant_grow_3, m_parent, m_mgr);
					break;
				}
			}
		}
	}
	std::list<std::list<plant>::iterator>::iterator rmite;
	for (rmite = remove_list.begin(); rmite != remove_list.end(); ++rmite) {
		m_plant_list.erase((*rmite));
	}
}

void game_logic::tickDisplay() {
	// update display (UI)
	for (unsigned int i = 0; i < MAX_CLIENT; ++i) {
		{ // energy per player
			std::string name;
			std::string value;
			{
				float& energy = m_player_energy.at(i);
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
						if (ite->m_plant_t == plant::harvester) {
							nb_fruit++;
						}
					}
				}
				std::stringstream ss("");
				ss << nb_plant;
				m_player_plant_map[i] = nb_plant;
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

void game_logic::tickEnd() {
	int count_player = 0;
	int count_enemy = 0;
	if (m_counter++ < 100)
		return;
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
		parameter_set::instance()->setValue(std::string("biolite.game.end"),
											std::string("defeat"));
	if (static_cast<float> (count_enemy) * nrj_enemy == 0.0f)
		parameter_set::instance()->setValue(std::string("biolite.game.end"),
											std::string("victory"));
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
	std::string mode = parameter_set::instance()->getValue("biolite.action.type");
	if ((mode == std::string("dryad")) || (mode == std::string("harvester"))) {
		if (checkPlant(hit)) {
			m_plant_flyby.add(plant::ghost_green, m_parent, m_mgr);
		} else {
			m_plant_flyby.add(plant::ghost_red, m_parent, m_mgr);
		}
	}
	if (mode == std::string("fetch")) {
		// put a green tiny sphere on the location if ok
		if (checkFruit(hit)) {
			m_plant_flyby.add(plant::sphere_green, m_parent, m_mgr);
		} else {
			m_plant_flyby.add(plant::sphere_red, m_parent, m_mgr);
		}
	}
	if (mode == std::string("damager")) {
		// put a green sphere on the location if ok
		if (checkEnemy(hit)) {
			m_plant_flyby.add(plant::big_sphere_green, m_parent, m_mgr);
		} else {
			m_plant_flyby.add(plant::big_sphere_red, m_parent, m_mgr);
		}
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

bool game_logic::checkEnemy(const irr::core::vector3df& hit, int player_id) {
	float closest = 1.0f;
	float closest_mine = 1.0f;
	int current_count = 0;
	// check you are in range of one of your dryad
	if (!checkDryad(hit, player_id)) return false;
	// check distance
	for (const auto& p : m_plant_list) {
		auto pos = p.m_position;
		float distance = (hit - pos).getLength();
		if (distance < closest) {
			closest = distance;
			if (p.m_player_id != player_id)
				closest_mine = distance;
		}
		if (p.m_player_id != player_id) current_count++;
	}
	if (current_count)
		if (closest_mine < m_max_dist)
			return true;
	return false;
}

bool game_logic::checkDryad(const irr::core::vector3df& hit, int player_id) {
	float closest = 1.0f;
	float closest_mine = 1.0f;
	int current_count = 0;
	// check distance
	for (const auto& p : m_plant_list) {
		auto pos = p.m_position;
		float distance = (hit - pos).getLength();
		if (distance < closest) {
			closest = distance;
			if (p.m_player_id == player_id)
				if (p.m_plant_mesh_t == plant::mama_tree)
					closest_mine = distance;
		}
		if (p.m_player_id == player_id) current_count++;
	}
	// check if current player has any plant
	if (current_count)
		if (closest_mine > m_max_dist)
			return false;
	if (closest < m_min_dist) return false;
	return true;
}

bool game_logic::checkFruit(const irr::core::vector3df& hit, int player_id) {
	float closest = 1.0f;
	float closest_mine = 1.0f;
	int current_count = 0;
	// check distance
	for (const auto& p : m_plant_list) {
		auto pos = p.m_position;
		float distance = (hit - pos).getLength();
		if (distance < closest) {
			closest = distance;
			if (p.m_player_id == player_id)
				if (p.m_plant_mesh_t == plant::fruit_tree)
					closest_mine = distance;
		}
		if (p.m_player_id == player_id) current_count++;
	}
	// check if current player has any plant
	if (current_count)
		if (closest_mine < m_min_dist)
			return true;
	return false;
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
	m_player_energy.at(player_win) += m_player_energy.at(player_lose);
	m_player_energy.at(player_lose) = 0;
	std::list<plant>::iterator ite;
	for (ite = m_plant_list.begin();
		ite != m_plant_list.end();
		++ite)
		if (ite->m_player_id == player_lose)
			ite->m_hit = true;
}