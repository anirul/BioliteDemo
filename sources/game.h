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

#ifndef BIOLITE_GAME_HEADER_DEFINED
#define BIOLITE_GAME_HEADER_DEFINED

#include "main.h"
#include "world.h"

class game_state;
class game_state_xml_still;
class game_state_xml_gui;

class game : public parameter_set_listener {
public :
	// world
	world* m_pworld;
	irr::IrrlichtDevice* m_pdevice;
	irr::scene::ISceneNodeAnimatorCameraArcball* m_parc;
	irr::scene::ISceneNode* m_camera_node;
	irr::scene::ICameraSceneNode* m_camera;
	irr::scene::ICameraSceneNode* m_arc_cam;
	irr::gui::IGUIFont* m_font;
public :
	//! init a game on a device with a xml description file
	game(
		irr::IrrlichtDevice* pdevice, 
		const std::string& xmlFile = XML_MENU_FILE);
	virtual ~game();
public :
	//! from parameter_set_listener
	virtual void operator()(const std::string& name, const std::string& value);
public :
	//! need
	//! biolite.camera.arcball.mouse
	void createWorld(const std::string& planet_file);
	//! need
	//! biolite.camera.arcball.mouse
	//! biolite.world.seed
	void createWorld(unsigned long seed);
	//! need
	//! biolite.font.small
	//! biolite.font.medium
	//! biolite.font.large
	void loadFont();
	bool OnEvent(const irr::SEvent& event);
	void render(irr::IrrlichtDevice* pdevice);
	void registerState(game_state* state);
private :
	//! need
	//! biolite.menu.current
	void changeState(const std::string& name);
	game_state* getCurrentState();
	std::map<std::string, game_state*> m_registered_state;
	irr::scene::ISceneNodeAnimator* m_camera_anim;
	friend class game_state;
	game_state* m_pstate;
};

class game_state {
public :
	// for Irrlicht engine
	virtual bool OnEvent(game* pg, const irr::SEvent& event) { return false; }
	virtual void render(game* pg, irr::IrrlichtDevice* pdevice) {}
	// added for XML based states
	virtual const std::string getName() const { 
		return std::string("game_state"); 
	}
protected :
	virtual void changeState(game* pg, const std::string& name) {
		pg->changeState(name);
	}
};

#endif // BIOLITE_GAME_HEADER_DEFINED

