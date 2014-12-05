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

#ifndef BIOLITE_PLANET_HEADER_DEFINED
#define BIOLITE_PLANET_HEADER_DEFINED

#include "game_math.h"
#include "parameter_set.h"
#include "main.h"

class planet :
public alea,
public irr::scene::ISceneNode,
public parameter_set_listener {
protected:
	// for ISceneNode
	irr::core::aabbox3d<irr::f32> Box;
	irr::video::S3DVertex* Vertices;
	irr::video::SMaterial Material;
	// mesh objects
	irr::scene::CIndexBuffer*  m_index_buffer;
	irr::scene::CVertexBuffer*  m_vertex_buffer;
	irr::scene::CDynamicMeshBuffer* m_mesh_buffer;
	irr::scene::SMesh* m_mesh;
	// internal mess
	std::vector<unsigned short> m_vpm;
	// private methods
	unsigned short addvertex(irr::video::S3DVertex v);
	unsigned short addvertex(unsigned short i1, unsigned short i2);
	float m_highest;
	void addtriangle(unsigned short i1, unsigned short i2, unsigned short i3);
	void subdivide(unsigned short ipos);
	//! ground average
	float m_ga;
	//! computation percentage
	unsigned int m_gen_done;
	//! number of step per frame to compute
	unsigned int m_gen_step;
	//! number of total step to compute
	unsigned int m_gen_size;
	//! normal compute percentage
	unsigned int m_normal_done;
public:
	//! from parameter_set_listener
	virtual void operator()(const std::string& name, const std::string& value);
public:
	//! colors
	irr::video::SColor m_ground;
	irr::video::SColor m_grass;
	irr::video::SColor m_high;
	//! vertices & indices
	std::vector<irr::video::S3DVertex> m_vvert;
	std::vector<unsigned short> m_vind;
	//! seed (name)
	std::string m_seed;
public:
	//! constructor
	planet(
		   irr::scene::ISceneNode* parent,
		   irr::scene::ISceneManager* mgr,
		   const std::string& seed,
		   const irr::video::SColor& ground = irr::video::SColor(0xffaaaa00), // yellow
		   const irr::video::SColor& grass = irr::video::SColor(0xff22aa22), // green
		   const irr::video::SColor& high = irr::video::SColor(0xffffffff), // white
		   unsigned int gen_step = GENSTEP,
		   unsigned int gen_size = NBGEN);
	//! destructor
	virtual ~planet();
	//! from ISceneNode : call on registering
	virtual void OnRegisterSceneNode();
	//! from ISceneNode : call on render
	virtual void render();
	//! from ISceneNode : get the bounding box
	virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox() const { return Box; }
	//! from ISceneNode : get material count (here 1)
	virtual irr::u32 getMaterialCount() const { return 1; }
	//! from ISceneNode : get a material from index (only 1 here so always the same)
	virtual irr::video::SMaterial& getMaterial(irr::u32 i) { return Material; }
	protected :
	//! get the list of vertices (is it still in use?)
	std::vector<irr::video::S3DVertex>& get_vert() { return m_vvert;	}
	//! get the list of indexes (is it still in use?)
	std::vector<unsigned short>& get_ind() { return m_vind; }
	//! get the vertices size (is it still used?)
	unsigned int get_vert_size() const { return m_vvert.size(); }
	//! get the highest point on the planet
	float get_highest();
public:
	//! reset planet
	void reset();
	//! from icosahedron to a more complexe sphere
	void subdivide();
	//! generate the planet fractal return generate (start)0.0..(end)1.0
	float generate();
	//! compute the normals
	float compute_normals();
	//! compute the colors (from the color ground grass and high)
	void compute_color();
	//! change the proportions ground/water return true when finished
	bool set_ground_average(float ga);
	//! return the proportions ground/water
	float get_ground_average() const;
	//! return the mesh with the planet (if calcul is finished)
	irr::scene::IMesh* get_mesh();
};

#endif

