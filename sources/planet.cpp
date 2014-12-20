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

#include "main.h"
#include "planet.h"

#ifndef MIN_MACRO
#define MIN_MACRO(a, b) (((a) < (b)) ? (a) : (b))
#endif // MIN_MACRO

#ifndef MAX_MACRO
#define MAX_MACRO(a, b) (((a) > (b)) ? (a) : (b))
#endif // MAX_MACRO

planet::planet(
	irr::scene::ISceneNode* parent, 
	irr::scene::ISceneManager* mgr, 
	const std::string& seed,
	const irr::video::SColor& ground,
	const irr::video::SColor& grass,
	const irr::video::SColor& high, 
	unsigned int gen_step,
	unsigned int gen_size) 
	:	irr::scene::ISceneNode(parent, mgr),
	 	m_index_buffer(NULL),
	 	m_vertex_buffer(NULL),
	 	m_mesh_buffer(NULL),
	 	m_mesh(NULL),
		m_tex0(nullptr),
		m_tex1(nullptr),
		m_ga(-1.0),
		m_gen_done(0),
		m_gen_step(gen_step),
		m_gen_size(gen_size),
		m_normal_done(0),
	 	m_ground(ground),
		m_grass(grass),
		m_high(high),
		m_seed(seed)
{
	// Box reset
	Box.reset(0.0f, 0.0f, 0.0f);
	Material.Lighting = true;
	Material.Wireframe = false;
	m_highest = 0.0f;
	set_seed(hash(seed));

	// generate icosahedron
	float t = (1 + sqrtf(5.0f))/2.0f;
	float s = sqrtf(1 + t*t);
	// create the 12 vertices
	addvertex(irr::video::S3DVertex(
			irr::core::vector3df(t/s, 1/s, 0), 
			irr::core::vector3df(t/s, 1/s, 0), 
			ground, 
			irr::core::vector2df(0.0f, 0.0f)));
	addvertex(irr::video::S3DVertex(
			irr::core::vector3df(-t/s, 1/s, 0), 
			irr::core::vector3df(-t/s, 1/s, 0), 
			ground, 
			irr::core::vector2df(0.0f, 0.0f)));
	addvertex(irr::video::S3DVertex(
			irr::core::vector3df(t/s, -1/s, 0), 
			irr::core::vector3df(t/s, -1/s, 0), 
			ground, 
			irr::core::vector2df(0.0f, 0.0f)));
	addvertex(irr::video::S3DVertex(
			irr::core::vector3df(-t/s, -1/s, 0), 
			irr::core::vector3df(-t/s, -1/s, 0), 
			ground, 
			irr::core::vector2df(0.0f, 0.0f)));
	addvertex(irr::video::S3DVertex(
			irr::core::vector3df(1/s, 0, t/s), 
			irr::core::vector3df(1/s, 0, t/s), 
			ground, 
			irr::core::vector2df(0.0f, 0.0f)));
	addvertex(irr::video::S3DVertex(
			irr::core::vector3df(1/s, 0, -t/s), 
			irr::core::vector3df(1/s, 0, -t/s), 
			ground, 
			irr::core::vector2df(0.0f, 0.0f)));
	addvertex(irr::video::S3DVertex(
			irr::core::vector3df(-1/s, 0, t/s), 
			irr::core::vector3df(-1/s, 0, t/s), 
			ground, 
			irr::core::vector2df(0.0f, 0.0f)));
	addvertex(irr::video::S3DVertex(
			irr::core::vector3df(-1/s, 0, -t/s), 
			irr::core::vector3df(-1/s, 0, -t/s), 
			ground, 
			irr::core::vector2df(0.0f, 0.0f)));
	addvertex(irr::video::S3DVertex(
			irr::core::vector3df(0, t/s, 1/s), 
			irr::core::vector3df(0, t/s, 1/s), 
			ground, 
			irr::core::vector2df(0.0f, 0.0f)));
	addvertex(irr::video::S3DVertex(
			irr::core::vector3df(0, -t/s, 1/s), 
			irr::core::vector3df(0, -t/s, 1/s), 
			ground, 
			irr::core::vector2df(0.0f, 0.0f)));
	addvertex(irr::video::S3DVertex(
			irr::core::vector3df(0, t/s, -1/s), 
			irr::core::vector3df(0, t/s, -1/s), 
			ground, 
			irr::core::vector2df(0.0f, 0.0f)));
	addvertex(irr::video::S3DVertex(
			irr::core::vector3df(0, -t/s, -1/s), 
			irr::core::vector3df(0, -t/s, -1/s), 
			ground, 
			irr::core::vector2df(0.0f, 0.0f)));

	// create the 20 triangles
	addtriangle(0, 8, 4);
	addtriangle(1, 10,7);
	addtriangle(2, 9, 11);
	addtriangle(7, 3, 1);    
	addtriangle(0, 5, 10);
	addtriangle(3, 9, 6);
	addtriangle(3, 11,9);
	addtriangle(8, 6, 4);   
	addtriangle(2, 4, 9);
	addtriangle(3, 7, 11);
	addtriangle(4, 2, 0);
	addtriangle(9, 4, 6);    
	addtriangle(2, 11,5);
	addtriangle(0, 10,8);
	addtriangle(5, 0, 2);
	addtriangle(10,5, 7);    
	addtriangle(1, 6, 8);
	addtriangle(1, 8, 10);
	addtriangle(6, 1, 3);
	addtriangle(11,7, 5);
	
	parameter_set::instance()->addListener(this);
}

planet::~planet() {
	parameter_set::instance()->removeListener(this);
}

void planet::operator()(
	const std::string& name, 
	const std::string& value) 
{
	// TODO
}

unsigned short planet::addvertex(unsigned short i1, unsigned short i2) {
	assert(i1 != i2);
	for (unsigned int i = 0; i < m_vpm.size(); i += 3)
		if (m_vpm[i] == MIN_MACRO(i1, i2) && m_vpm[i + 1] == MAX_MACRO(i1, i2))
			return m_vpm[i + 2];
	m_vpm.push_back(MIN_MACRO(i1, i2));
	m_vpm.push_back(MAX_MACRO(i1, i2));
	irr::video::S3DVertex norm_vert;
	norm_vert.Pos = m_vvert[i1].Pos + m_vvert[i2].Pos;
	norm_vert.Pos.normalize();
	norm_vert.Normal = m_vvert[i1].Normal + m_vvert[i2].Normal;
	norm_vert.Normal.normalize();
	norm_vert.Color = m_ground;
	norm_vert.TCoords.Y = (float)(norm_vert.Normal.Y);
	norm_vert.TCoords.X =
		(float)fabs(atan2(norm_vert.Normal.X, norm_vert.Normal.Z) / M_PI);
	unsigned short temp = addvertex(norm_vert);
	m_vpm.push_back(temp);
	return m_vpm[m_vpm.size() - 1];
}

unsigned short planet::addvertex(irr::video::S3DVertex v) {
	v.TCoords.Y = (float)(v.Normal.Y);
	v.TCoords.X = (float)fabs(atan2(v.Normal.X, v.Normal.Z) / M_PI);
	std::cout << v.TCoords.X << std::endl;
	m_vvert.push_back(v);
	Vertices = &m_vvert[0];
	Box.addInternalPoint(v.Pos);
	return m_vvert.size() - 1;
}

void planet::addtriangle(
	unsigned short i1, 
	unsigned short i2, 
	unsigned short i3) 
{
	m_vind.push_back(i1);
	m_vind.push_back(i2);
	m_vind.push_back(i3);
}

void planet::subdivide(unsigned short ipos) {
	// normalize
	int nv1 = addvertex(m_vind[ipos], m_vind[ipos + 1]);
	int nv2 = addvertex(m_vind[ipos + 1], m_vind[ipos + 2]);
	int nv3 = addvertex(m_vind[ipos + 2], m_vind[ipos]);
	// triangle 1
	addtriangle(m_vind[ipos], nv1, nv3);
	// triangle 2
	addtriangle(nv1, m_vind[ipos + 1], nv2);
	// triangle 3
	addtriangle(nv3, nv2, m_vind[ipos + 2]);
	// triangle 4
	addtriangle(nv1, nv2, nv3);
}

void planet::subdivide() {
	int ind = m_vind.size();
	for (int j = 0; j < ind; j += 3)
		subdivide(j);
	std::vector<unsigned short> tpi;
	for (unsigned int i = ind; i < m_vind.size(); ++i)
		tpi.push_back(m_vind[i]);
	m_vind = tpi;
}

float planet::generate() {
	if (m_gen_step == 0) return 1.0f;
	if (m_gen_size == 0) return 1.0f;
	if (m_gen_done == m_gen_size) return 1.0f;
	if (m_mesh)	m_mesh->drop();
	m_mesh = NULL;
	for (unsigned int i = 0; i < m_gen_step; ++i) {
		irr::core::vector3df vec;
		// -1 or +1
		float s = (stdmin() % 2 - 0.5f) * 2.0f;
		do {
			vec.X = (stdmin() % 2000 - 1000) / 1000.0f;
			vec.Y = (stdmin() % 2000 - 1000) / 1000.0f;
			vec.Z = (stdmin() % 2000 - 1000) / 1000.0f;
		} while (vec.getLength() > 1.0f);
		for (unsigned int j = 0; j < m_vvert.size(); ++j) {
			float l = m_vvert[j].Pos.getLength();
			if ((m_vvert[j].Pos - vec).dotProduct(vec) > 0.0f)
				l += 0.0005f * s;
			else
				l -= 0.0005f * s;
			m_vvert[j].Pos = m_vvert[j].Normal * l;
			if (l > m_highest) m_highest = l;
		}
	}
	m_gen_done += m_gen_step;
	return (float)m_gen_done / (float)m_gen_size;
}

float planet::compute_normals() {
	// FIXME
	if (m_normal_done + 2 >= m_vvert.size()) return 1.0f;
	if (m_mesh)	m_mesh->drop();
	m_mesh = NULL;
	// Calculate Normals go through all vertex
	for (unsigned int nvert = m_normal_done; nvert < m_vvert.size(); ++nvert) {
		m_vvert[nvert].Normal = irr::core::vector3df(0.0f, 0.0f, 0.0f);
		// go through all triangles
		for (unsigned int nind = 0; nind < m_vind.size(); nind += 3) {
			if ((m_vind[nind] == nvert) ||
				 (m_vind[nind + 1] == nvert) ||
				 (m_vind[nind + 2] == nvert))
			{
				irr::core::vector3df v1 = 
					m_vvert[m_vind[nind]].Pos - m_vvert[m_vind[nind + 1]].Pos;
				irr::core::vector3df v2 = 
					m_vvert[m_vind[nind]].Pos - m_vvert[m_vind[nind + 2]].Pos;
				m_vvert[nvert].Normal += (v1.crossProduct(v2)).normalize();
			}
		}
		m_vvert[nvert].Normal.normalize();
		if (nvert > m_normal_done + m_gen_step) {
			m_normal_done += m_gen_step;
			break;
		}
	}
	return (float)m_normal_done / (float)m_vvert.size();
}

float planet::get_highest() {
	if (m_highest == 0.0f) {
		for (unsigned int i = 0; i < m_vvert.size(); ++i) {
			float length = m_vvert[i].Pos.getLength();
			if (length > m_highest)
				m_highest = length;
		}
	}
	return m_highest;
}

void planet::compute_color() {
	const float snow_hight = 2.0f / 3.0f;
	const float delta = get_highest() - 1.0f;
	if (m_mesh)	m_mesh->drop();
	m_mesh = NULL;
	// Calculate Color go through all vertex
	for (unsigned int nvert = 0; nvert < m_vvert.size(); ++nvert) {
		float equator = fabs(m_vvert[nvert].Normal.Y);
		float hight = m_vvert[nvert].Pos.getLength() - 1.0f;
		m_vvert[nvert].Color = m_grass.getInterpolated(m_ground, equator);
		// top of montains
		if (hight > snow_hight * delta) 
			m_vvert[nvert].Color = m_high;
		// underwater
		if (hight < 0.0f)
			m_vvert[nvert].Color = m_ground;
		float poles = 1.0f - equator;
		if (poles < 0.1)
			m_vvert[nvert].Color = m_high;
	}
}

float planet::get_ground_average() const {
	float ground = 0.0f;
	for (unsigned int i = 0; i < m_vvert.size(); ++ i) {
		float l = m_vvert[i].Pos.getLength();
		if (l > 1.0f)
			ground += 1.0f;
	}
	return ground / m_vvert.size();
}

bool planet::set_ground_average(float ga) {
	// used to avoid oscilliating aroud target
	static bool up = false;
	static int count = 0;
	if (m_ga == ga) return true;
	if (m_mesh)	m_mesh->drop();
	m_mesh = NULL;
	m_ga = get_ground_average();
	// check around a 5 per thousand
	if ((fabs(m_ga - ga) < 0.001) || (count == 10)) {
		count = 0;
		m_ga = ga;
		// recalculate colors
		compute_color();
		return true;
	}
	float change = 0.0f;
	if (m_ga < ga) {
		if (!up) count++;
		up = true;
		change = 0.0001f;
	} else {
		if (up) count++;
		up = false;
		change = -0.0001f;
	}
	// change height
	for (unsigned int j = 0; j < m_vvert.size(); ++j) {
		float l = m_vvert[j].Pos.getLength();
		l += change;
		irr::core::vector3df normal = m_vvert[j].Pos;
		normal.normalize();
		m_vvert[j].Pos = normal * l;
	}
	return false;
}

void planet::reset() {
	for (unsigned int j = 0; j < m_vvert.size(); ++j) {
		m_vvert[j].Pos.normalize();
		m_vvert[j].Normal = m_vvert[j].Pos;
		m_vvert[j].Color = m_ground;
		m_gen_done = 0;
		m_normal_done = 0;
	}
	if (m_mesh) m_mesh->drop();
	m_mesh = NULL;
}

irr::scene::IMesh* planet::get_mesh() {
	if (m_mesh) return m_mesh;
	// FIXME allocation bug?
	m_index_buffer = new irr::scene::CIndexBuffer(
		irr::video::EIT_16BIT);
	m_vertex_buffer = new irr::scene::CVertexBuffer(
		irr::video::EVT_STANDARD);
	if (!m_mesh_buffer)
		m_mesh_buffer = new	 irr::scene::CDynamicMeshBuffer(
			irr::video::EVT_STANDARD,
			irr::video::EIT_16BIT);
	m_mesh = new irr::scene::SMesh();
	// fill up the index buffer
	for (unsigned short ind = 0; ind < m_vind.size(); ++ind)
		m_index_buffer->push_back(m_vind[ind]);
	for (unsigned short ind = 0; ind < m_vvert.size(); ++ind)
		m_vertex_buffer->push_back(m_vvert[ind]);
	m_mesh_buffer->setIndexBuffer(m_index_buffer);
	m_mesh_buffer->setVertexBuffer(m_vertex_buffer);
	m_mesh_buffer->recalculateBoundingBox();
	m_mesh->addMeshBuffer(m_mesh_buffer);
	m_mesh->recalculateBoundingBox();
	return m_mesh;
}

void planet::set_textures(
	irr::IrrlichtDevice* pdevice,
	const std::string& st0,
	const std::string& st1)
{
	auto* pvideo = pdevice->getVideoDriver();
	if (st0 != std::string("")) {
		m_tex0 = pvideo->getTexture(st0.c_str());
	} else {
		if (m_tex0) m_tex0->drop();
		m_tex0 = nullptr;
	}
	if (st1 != std::string("")) {
		m_tex1 = pvideo->getTexture(st1.c_str());
	} else {
		if (m_tex1) m_tex1->drop();
		m_tex1 = nullptr;
	}
}

// inherited from ISceneNode
void planet::render() {
	if (IsVisible) {
		irr::video::IVideoDriver* pvideo = SceneManager->getVideoDriver();
		if (m_tex0) {
			Material.setTexture(0, m_tex0);
			Material.setFlag(irr::video::EMF_TRILINEAR_FILTER, true);
			Material.MaterialType = irr::video::EMT_SOLID;
		}
		if (m_tex1) {
			Material.setTexture(1, m_tex1);
			Material.setFlag(irr::video::EMF_TRILINEAR_FILTER, true);
		}
		if (m_tex0 && m_tex1) {
			auto* ps = parameter_set::instance();
			auto i = (int)ps->getValueDouble("biolite.planet.texture.type");
			Material.MaterialType = (irr::video::E_MATERIAL_TYPE)i;
		}
		pvideo->setMaterial(Material);
		pvideo->setTransform(irr::video::ETS_WORLD, AbsoluteTransformation);
		pvideo->drawIndexedTriangleList(
			&m_vvert[0], 
			m_vvert.size(), 
			&m_vind[0],
			m_vind.size() / 3);
	}
}

// inherited from ISceneNode
void planet::OnRegisterSceneNode() {
	if (isVisible())
		SceneManager->registerNodeForRendering(this);
	ISceneNode::OnRegisterSceneNode();
}
