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

#include "ai.h"
#include "game_logic.h"

//! easy ai
//! try to click once per tick
void easy_ai::tick(int player_id, int plant_count) {
	if (m_had_plant[player_id]) {
		if (plant_count == 0) return;
	} else {
		if (plant_count > 0) m_had_plant[player_id] = true;
	}
	irr::core::line3df ray;
	irr::core::vector3df hit;
	ray.start = irr::core::vector3df(
		(float)(rand() % 100) - 50.0f, 
		(float)(rand() % 100) - 50.0f, 
		(float)(rand() % 100) - 50.0f);
	ray.end = irr::core::vector3df(
		0.0f, 0.0f, 0.0f);
	if (m_pw->intersect(ray, hit)) {
		click_desc cd;
		cd.m_position = hit;
		cd.m_player_id = player_id;
		cd.m_click_t = click_desc::ct_harvester;
		if (!(rand() % 6))
			cd.m_click_t = click_desc::ct_damager;
		if (!(rand() % 10))
			cd.m_click_t = click_desc::ct_dryad;
		m_gl->planetClick(cd);
		cd.m_click_t = click_desc::ct_fetch;
		m_gl->planetClick(cd);
	}
	std::cout << "miss" << std::endl;
}

//! medium ai
//! try # for a successfull click once per tick
void medium_ai::tick(int player_id, int plant_count) {
	if (m_had_plant[player_id]) {
		if (plant_count == 0) return;
	} else {
		if (plant_count > 0) m_had_plant[player_id] = true;
	}
	irr::core::line3df ray;
	irr::core::vector3df hit;
	for (int i = 0; i < 5; ++i) {
		ray.start = irr::core::vector3df(
			(float)(rand() % 100) - 50.0f, 
			(float)(rand() % 100) - 50.0f, 
			(float)(rand() % 100) - 50.0f);
		ray.end = irr::core::vector3df(
			0.0f, 0.0f, 0.0f);
		if (m_pw->intersect(ray, hit)) break;
	}
	click_desc cd;
	cd.m_position = hit;
	cd.m_player_id = player_id;
	cd.m_click_t = click_desc::ct_harvester;
	if (!(rand() % 10))
		cd.m_click_t = click_desc::ct_dryad;
	m_gl->planetClick(cd);
	cd.m_click_t = click_desc::ct_fetch;
	m_gl->planetClick(cd);
}

//! hard ai
//! try # for a successfull click once per tick
//! TODO do better probably
void hard_ai::tick(int player_id, int plant_count) {
	if (m_had_plant[player_id]) {
		if (plant_count == 0) return;
	} else {
		if (plant_count > 0) m_had_plant[player_id] = true;
	}
	irr::core::line3df ray;
	irr::core::vector3df hit;
	for (int i = 0; i < 10; ++i) {
		ray.start = irr::core::vector3df(
			(float)(rand() % 100) - 50.0f, 
			(float)(rand() % 100) - 50.0f, 
			(float)(rand() % 100) - 50.0f);
		ray.end = irr::core::vector3df(
			0.0f, 0.0f, 0.0f);
		if (m_pw->intersect(ray, hit)) break;
	}
	click_desc cd;
	cd.m_position = hit;
	cd.m_player_id = player_id;
	cd.m_click_t = click_desc::ct_harvester;
	if (!(rand() % 3))
		cd.m_click_t = click_desc::ct_damager;
	if (!(rand() % 6))
		cd.m_click_t = click_desc::ct_dryad;
	m_gl->planetClick(cd);
	cd.m_click_t = click_desc::ct_fetch;
	m_gl->planetClick(cd);
}
