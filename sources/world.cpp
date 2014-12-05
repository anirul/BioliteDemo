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
#include "world.h"
#include "media_collection.h"
#include "parameter_set.h"
#include "xml_planet.h"

world::world(irr::IrrlichtDevice* pdevice, const std::string& planet_file) {
	m_sun_pos = irr::core::vector3df(100.0f, 0.0f, 0.0f);
	m_seed = INITIAL_SEED;
	m_planet = NULL;
	m_pwater = NULL;
	m_sun = NULL;
	m_ground_average = 0.5f;
	m_pwater = NULL;
	m_light = NULL;
	m_anim = NULL;
	m_selector = NULL;
	m_mesh_node = NULL;
	// get the scene manager
	irr::scene::ISceneManager* pscene = pdevice->getSceneManager();
	m_scene = pscene;
	irr::video::IVideoDriver* pvideo = pdevice->getVideoDriver();
	// create the skybox
	m_skybox = pscene->addSkyBoxSceneNode(
		pvideo->getTexture(getPathOfMedia(parameter_set::instance()->getValue(
					"biolite.skybox.up").c_str()).c_str()),
		pvideo->getTexture(getPathOfMedia(parameter_set::instance()->getValue(
					"biolite.skybox.down").c_str()).c_str()),
		pvideo->getTexture(getPathOfMedia(parameter_set::instance()->getValue(
					"biolite.skybox.right").c_str()).c_str()),
		pvideo->getTexture(getPathOfMedia(parameter_set::instance()->getValue(
					"biolite.skybox.left").c_str()).c_str()),
		pvideo->getTexture(getPathOfMedia(parameter_set::instance()->getValue(
					"biolite.skybox.front").c_str()).c_str()),
		pvideo->getTexture(getPathOfMedia(parameter_set::instance()->getValue(
					"biolite.skybox.back").c_str()).c_str()));
	m_skybox->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	// create the sun and attach a light to it
	m_sun = pscene->addBillboardSceneNode(
		m_skybox,
		irr::core::dimension2d<irr::f32>(30, 30));
	m_sun->setPosition(m_sun_pos);
	m_sun->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	m_sun->setMaterialType(irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL);
	m_sun->setMaterialTexture(0, pvideo->getTexture(
		getPathOfMedia(parameter_set::instance()->getValue(
					"biolite.sun").c_str()).c_str()));
	m_light = pscene->addLightSceneNode(m_sun);
	pscene->setAmbientLight(irr::video::SColorf(0.2f, 0.2f, 0.2f, 0.2f));
	// create animator
	m_anim = pscene->createRotationAnimator(irr::core::vector3df(0.0f, 0.05f, 0.0f));
	// create the fake planet node
	m_planet_node = pdevice->getSceneManager()->addEmptySceneNode(
		pdevice->getSceneManager()->getRootSceneNode());
	// create the ground
	m_planet = xml_planet::parsePlanet(
		planet_file,
		m_planet_node, 
		pdevice->getSceneManager());
	m_seed = hash(m_planet->m_seed);
	// create the water surface
	m_pwater = new planet(
		m_planet_node, 
		pdevice->getSceneManager(), 
		std::string("water"),
		irr::video::SColor(0x7f00007f)); // blue
	m_pwater->remove();
	for (int i = 0; i < NBSUB; ++i) 
		m_pwater->subdivide();
	//! new water surface
	irr::scene::IMesh* mesh = m_pwater->get_mesh();
	irr::scene::ISceneNode* node = 0;
	node = pscene->addWaterSurfaceSceneNode(mesh, 0.0005f, 200.0f, 0.001f);
	irr::video::SMaterial mat_water;
	mat_water.DiffuseColor.setRed(51);
	mat_water.DiffuseColor.setGreen(51);
	mat_water.DiffuseColor.setBlue(178);
	mat_water.DiffuseColor.setAlpha(128);
	mat_water.SpecularColor.setRed(255);
	mat_water.SpecularColor.setGreen(255);
	mat_water.SpecularColor.setBlue(255);
	mat_water.SpecularColor.setAlpha(255);
	mat_water.Shininess = 50.0f;
	mat_water.AmbientColor = mat_water.DiffuseColor;
	mat_water.MaterialType = irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL;
	node->getMaterial(0) = mat_water;
	m_skybox->addAnimator(m_anim);
	// create the poles marquers
	irr::scene::IMesh* cylinder = 
		pdevice->getSceneManager()->getGeometryCreator()->createCylinderMesh(
			0.01f, 2.5f, 4, 0x77777777);
	irr::scene::ISceneNode* cylinder_node = 
		pdevice->getSceneManager()->addMeshSceneNode(cylinder, m_planet_node);
	cylinder_node->setPosition(irr::core::vector3df(0.0f, -1.25, 0.0f));
	cylinder_node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	cylinder_node->setMaterialType(irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL);
	// listen to the parameter_set
	parameter_set::instance()->addListener(this);
}

irr::core::vector3df world::getSunPosition() const {
	return m_sun->getAbsolutePosition();
}

world::~world() {
	parameter_set::instance()->removeListener(this);
	if (m_planet) 
		m_planet->drop();
	if (m_pwater) 
		m_pwater->drop();
}

void world::operator()(
	const std::string& name,
	const std::string& value) 
{
	if (name == std::string("biolite.planet.water")) {
		double percent = parameter_set::instance()->getValueDouble(name);
		percent *= 0.01;
		m_ground_average = (float)percent;
	}
	if (name == std::string("biolite.planet.color.low")) {
		if (m_planet) {
			m_planet->m_ground = string2color(value);
			m_planet->compute_color();
		}
	}
	if (name == std::string("biolite.planet.color.medium")) {
		if (m_planet) {
			m_planet->m_grass = string2color(value);
			m_planet->compute_color();
		}
	}
	if (name == std::string("biolite.planet.color.high")) {
		if (m_planet) {
			m_planet->m_high = string2color(value);
			m_planet->compute_color();
		}
	}
}

bool world::render_state(irr::IrrlichtDevice* pdevice) {
	m_finished = false;
	if (!m_planet) return false;
	if (m_planet->generate() < 1.0f) return false;
	if (!m_planet->set_ground_average(m_ground_average)) return false;
	if (m_planet->compute_normals() < 1.0f) return false;
	if (m_planet->get_mesh()) {
		if (!m_mesh_node) {
			irr::scene::ISceneManager* pscene = pdevice->getSceneManager();
			m_mesh_node = pscene->addMeshSceneNode(
				m_planet->get_mesh(),
				m_planet_node);
			if (m_selector) m_selector->drop();
			m_selector = pscene->createTriangleSelector(
				m_planet->get_mesh(), 
				m_mesh_node);
			std::string current = 
				parameter_set::instance()->getValue(
					std::string("biolite.menu.current"));
			if (current == "edit-planet") {
//				std::string file = m_planet->m_seed;
//				file.append(".xml");
//				std::ofstream ofs(file.c_str());
//				xml_planet::savePlanet(ofs, *m_planet);
//				ofs.close();
			}
		}
		return true;
	} 
	return false;
}

void world::render(irr::IrrlichtDevice* pdevice)
{
	m_finished = render_state(pdevice);
	if (!m_finished) {
		if (m_mesh_node) {
			m_mesh_node->remove();
			m_mesh_node = NULL;
		}
	}
	m_planet->setVisible(!m_finished);
}

void world::set_seed(long l) {
	if (m_seed == l) return;
	if (!m_scene) return;
	if (!m_planet) return;
	m_planet->set_seed(l);
	m_planet->reset();
	m_seed = l;
}

bool world::intersect(
	const irr::core::line3df& ray, 
	irr::core::vector3df& hit) 
{
	if (!m_finished) return false;
	irr::scene::ISceneCollisionManager* collision = 
		m_scene->getSceneCollisionManager();
	irr::core::triangle3df out;
	irr::scene::ISceneNode* node = NULL;
	collision->getCollisionPoint(ray, m_selector, hit, out, node);
	if (node) return true;
	return false;
}


