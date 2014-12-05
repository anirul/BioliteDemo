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

#ifndef BIOLITE_WORLD_HEADER_DEFINED
#define BIOLITE_WORLD_HEADER_DEFINED

#include "planet.h"
#include "plant.h"

class world : public parameter_set_listener {
public :
	planet* m_planet;
	planet* m_pwater;
	irr::scene::ISceneNode* m_planet_node;
protected :
	int m_seed;
	float m_ground_average;
	int m_last_pid;
	bool m_finished;
	irr::scene::ISceneManager* m_scene;
	irr::scene::ISceneNode* m_sun;
	irr::scene::ISceneNode* m_skybox;
	irr::scene::ISceneNodeAnimator* m_anim;
	irr::scene::ILightSceneNode* m_light;
	irr::scene::IMeshSceneNode* m_mesh_node;
	irr::scene::ITriangleSelector* m_selector;
	irr::core::vector3df m_sun_pos;
	irr::video::SMaterial m_material;
public :
	world(irr::IrrlichtDevice* pdevice, const std::string& planet_file);
	virtual ~world();
public :
	//! from parameter_set_listener
	virtual void operator()(const std::string& name, const std::string& value);
public :
	void set_ground_average(float ga) { m_ground_average = ga; }
	void render(irr::IrrlichtDevice* pdevice);
	void set_seed(long l);
	long get_seed() { return m_seed; }
	bool intersect(
		const irr::core::line3df& ray, 
		irr::core::vector3df& hit);
	irr::core::vector3df getSunPosition() const;
private :
	bool render_state(irr::IrrlichtDevice* pdevice);
};

#endif

