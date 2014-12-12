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
#include "plant.h"
#include "parameter_set.h"
#include "media_collection.h"

std::map<plant::plant_mesh_type, irr::scene::IAnimatedMesh*> 
	plant::s_plant_mesh;

std::map<int, irr::video::ITexture*> plant::s_texture_plant_map;
std::map<int, irr::video::ITexture*> plant::s_texture_fruit_map;
irr::video::ITexture* plant::s_texture_red = NULL;
irr::video::ITexture* plant::s_texture_green = NULL;

void plant::addMesh(
	irr::scene::ISceneManager* smgr,
	plant_mesh_type pmt, 
	const std::string& file)
{
	irr::scene::IAnimatedMesh* mesh = 
		smgr->getMesh(getPathOfMedia(file.c_str()).c_str());
	if (!mesh) {
		std::string error("PLANT ERROR : cannot load mesh : ");
		error.append(file);
		throw std::runtime_error(error);
	}
	s_plant_mesh.insert(std::make_pair(pmt,	mesh));
}

void plant::addPlantTexture(
	irr::IrrlichtDevice* pdevice,
	int player,
	const std::string& file)
{
	irr::video::ITexture* texture = 
		pdevice->getVideoDriver()->getTexture(
			getPathOfMedia(file.c_str()).c_str());
	if (!texture) {
		std::string error("PLANT ERROR : cannot load plant texture : ");
		error.append(file);
		throw std::runtime_error(error);
	}
	s_texture_plant_map.insert(std::make_pair(player, texture));
}

void plant::addFruitTexture(
	irr::IrrlichtDevice* pdevice,
	int player,
	const std::string& file)
{
	irr::video::ITexture* texture = 
		pdevice->getVideoDriver()->getTexture(
			getPathOfMedia(file.c_str()).c_str());
	if (!texture) {
		std::string error("PLANT ERROR : cannot load fruit texture : ");
		error.append(file);
		throw std::runtime_error(error);
	}
	s_texture_fruit_map.insert(std::make_pair(player, texture));
}

irr::video::ITexture* plant::addTexture(
	irr::IrrlichtDevice* pdevice,
	const std::string& file)
{
	irr::video::ITexture* texture = 
		pdevice->getVideoDriver()->getTexture(
			getPathOfMedia(file.c_str()).c_str());
	if (!texture) {
		std::string error("ERROR : cannot load texture : ");
		error.append(file);
		throw std::runtime_error(error);
	}
	return texture;
}

void plant::init(irr::IrrlichtDevice* pdevice) {
	parameter_set* ps = parameter_set::instance();
	irr::scene::ISceneManager* mgr = pdevice->getSceneManager();
	// load plant meshes
	plant::addMesh(
		mgr,
		plant::ghost_red,
		ps->getValue(std::string("biolite.ghost.mesh.red")));
	plant::addMesh(
		mgr,
		plant::ghost_green,
		ps->getValue(std::string("biolite.ghost.mesh.green")));
	plant::addMesh(
		mgr, 
		plant::plant_grow_1, 
		ps->getValue(std::string("biolite.plant.mesh.grow1")));
	plant::addMesh(
		mgr, 
		plant::plant_grow_2, 
		ps->getValue(std::string("biolite.plant.mesh.grow2")));
	plant::addMesh(
		mgr, 
		plant::plant_grow_3, 
		ps->getValue(std::string("biolite.plant.mesh.grow3")));
	plant::addMesh(
		mgr, 
		plant::plant_die_1, 
		ps->getValue(std::string("biolite.plant.mesh.die1")));
	plant::addMesh(
		mgr, 
		plant::plant_die_2, 
		ps->getValue(std::string("biolite.plant.mesh.die2")));
	plant::addMesh(
		mgr,
		plant::plant_die_3,
		ps->getValue(std::string("biolite.plant.mesh.die3")));
	// load fruit meshes
	plant::addMesh(
		mgr,
		plant::fruit_tree,
		ps->getValue(std::string("biolite.fruit.mesh")));
	plant::addMesh(
		mgr,
		plant::mama_tree,
		ps->getValue(std::string("biolite.mama.mesh")));
	// load plant textures
	plant::addPlantTexture(
		pdevice,
		0,
		ps->getValue(std::string("biolite.plant.texture.player0")));
	plant::addPlantTexture(
		pdevice,
		1,
		ps->getValue(std::string("biolite.plant.texture.player1")));
	plant::addPlantTexture(
		pdevice,
		2,
		ps->getValue(std::string("biolite.plant.texture.player2")));
	plant::addPlantTexture(
		pdevice,
		3,
		ps->getValue(std::string("biolite.plant.texture.player3")));
	// load fruit textures
	plant::addFruitTexture(
		pdevice,
		0,
		ps->getValue(std::string("biolite.fruit.texture.player0")));
	plant::addFruitTexture(
		pdevice,
		1,
		ps->getValue(std::string("biolite.fruit.texture.player1")));
	plant::addFruitTexture(
		pdevice,
		2,
		ps->getValue(std::string("biolite.fruit.texture.player2")));
	plant::addFruitTexture(
		pdevice,
		3,
		ps->getValue(std::string("biolite.fruit.texture.player3")));
	// ghost textures
	s_texture_red = plant::addTexture(
		pdevice, 
		ps->getValue(std::string("biolite.ghost.texture.red")));
	s_texture_green = plant::addTexture(
		pdevice, 
		ps->getValue(std::string("biolite.ghost.texture.green")));
}

plant::~plant() {
	remove();
}

void plant::remove() {
	if (m_plant_node) {
		m_plant_node->remove();
		m_plant_node = NULL;
	}
}

void plant::add(
	plant_mesh_type pmt,
	irr::scene::ISceneNode* parent,
	irr::scene::ISceneManager* mgr)
{
	if ((m_plant_mesh_t == pmt) &&
		(m_plant_mesh_t != ghost_red) &&
		(m_plant_mesh_t != ghost_green))
		return;
	m_plant_mesh_t = pmt;
	// remove the old one from the tree (if any)
	remove();
	m_plant_node = mgr->addAnimatedMeshSceneNode(
		s_plant_mesh[pmt],
		parent);
	float deltaY = (float)parameter_set::instance()->getValueDouble(
		"biolite.plant.delta.Y");
	irr::core::vector3df scale(
		(float)parameter_set::instance()->getValueDouble(
			"biolite.plant.mesh.scale.X"),
		(float)parameter_set::instance()->getValueDouble(
			"biolite.plant.mesh.scale.Y"),
		(float)parameter_set::instance()->getValueDouble(
			"biolite.plant.mesh.scale.Z"));
	m_plant_node->setScale(scale);
	irr::core::quaternion quat;
	irr::core::vector3df from(0.0f, 1.0f, 0.0f);
	irr::core::vector3df to(m_position);
	to.normalize();
	from.normalize();
	quat.normalize();
	quat.rotationFromTo(from, to);
	irr::core::matrix4 mat;
	mat.makeIdentity();
	mat.buildRotateFromTo(from, to);
	m_plant_node->setRotation(mat.getRotationDegrees());
	m_plant_node->setPosition((m_position * deltaY));
	if (m_plant_mesh_t == ghost_red) {
		m_plant_node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		m_plant_node->setMaterialType(irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL);
		m_plant_node->setMaterialTexture(0, s_texture_red);
		return;
	}
	if (m_plant_mesh_t == ghost_green) {
		m_plant_node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		m_plant_node->setMaterialType(irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL);
		m_plant_node->setMaterialTexture(0, s_texture_green);
		return;
	}
	if (m_plant_mesh_t == fruit_tree) {
		m_plant_node->setMaterialTexture(0, s_texture_fruit_map[m_player_id]);
		return;
	} 
	m_plant_node->setMaterialTexture(0, s_texture_plant_map[m_player_id]);
}
