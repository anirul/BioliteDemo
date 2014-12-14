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

// parameter_set values for init! :
//	MESH
//		biolite.plant.mesh.grow1
//		biolite.plant.mesh.grow2
//		biolite.plant.mesh.grow3
//		biolite.plant.mesh.die1
//		biolite.plant.mesh.die2
//		biolite.fruit.mesh
//		biolite.mama.mesh

#ifndef BIOLITE_PLANT_HEADER_DEFINED
#define BIOLITE_PLANT_HEADER_DEFINED

#include "main.h"

class plant {
public :
	enum plant_mesh_type {
		big_sphere_blue = -42,
		sphere_blue = -41,
		ghost_blue = -40,
		big_sphere_yellow = -32,
		sphere_yellow = -31,
		ghost_yellow = -30,
		big_sphere_green = -12,
		sphere_green = -11,
		ghost_green = -10,
		big_sphere_red = -22,
		sphere_red = -21,
		ghost_red = -20,
		null = -1,
		plant_grow_1 = 0,
		plant_grow_2 = 1,
		plant_grow_3 = 2,
		plant_die_1 = 10,
		plant_die_2 = 11,
		plant_die_3 = 12,
		fruit_tree = 32,
		mama_tree = 42
	};
	enum plant_type {
		dryad = 0,
		harvester = 20,
		damager = 40
	};
protected :
	irr::scene::IMeshSceneNode* m_plant_node;
protected :
	static void addMesh(
		irr::scene::ISceneManager* smgr,
		plant_mesh_type pmt, 
		const std::string& file);
	static void addSphere(
		irr::scene::ISceneManager* smgr,
		plant_mesh_type pmt,
		float radius);
	static void addPlantTexture(
		irr::IrrlichtDevice* pdevice,
		int player,
		const std::string& file);
	static void addFruitTexture(
		irr::IrrlichtDevice* pdevice,
		int player,
		const std::string& file);
	static irr::video::ITexture* addTexture(
		irr::IrrlichtDevice* pdevice,
		const std::string& file);
public :
	//! mesh store
	static std::map<plant_mesh_type, irr::scene::IMesh*>
		s_plant_mesh;
	//! alternate texture 1 map (per player)
	static std::map<int, irr::video::ITexture*> s_texture_plant_map;
	static std::map<int, irr::video::ITexture*> s_texture_fruit_map;
	static irr::video::ITexture* s_texture_red;
	static irr::video::ITexture* s_texture_green;
	static irr::video::ITexture* s_texture_yellow;
	static irr::video::ITexture* s_texture_blue;
	//! load the mesh
	static void init(irr::IrrlichtDevice* pdevice);
public :
	plant() :
	m_plant_node(NULL), m_fruit(false), m_plant_mesh_t(null), m_plant_t(dryad)
	{}
	virtual ~plant();
public :
	void remove();
	void add(
		plant_mesh_type pmt,
		irr::scene::ISceneNode* parent,
		irr::scene::ISceneManager* mgr);
public :
	irr::core::vector3df m_position;
	float m_energy;
	bool m_hit;
	bool m_update;
	bool m_fruit;
	int m_player_id;
	plant_mesh_type m_plant_mesh_t;
	plant_type m_plant_t;
};

#endif // BIOLITE_PLANT_HEADER_DEFINED

