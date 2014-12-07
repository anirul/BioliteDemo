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
#include "game.h"
#include "sound.h"
#include "media_collection.h"
#include "xml_image.h"
#include "xml_button.h"
#include "xml_menu.h"
#include "game_state_xml_still.h"
#include "game_state_xml_gui.h"
#include "parameter_set.h"
#include "CSceneNodeAnimatorCameraArcball.h"

//////////////////////////////////////////////////////////////////////////
// game implementation

game::game(irr::IrrlichtDevice* pdevice, const std::string& xmlFile) {
	// default variables
	m_pdevice = pdevice;
	m_parc = NULL;
	m_pstate = NULL;
	m_pworld = NULL;
	m_font = NULL;
	sound::instance(pdevice);
	
	// get screen size
	irr::video::IVideoDriver* pvideo = pdevice->getVideoDriver();
	irr::core::dimension2d<irr::u32> screen_size = pvideo->getScreenSize();
	irr::gui::IGUIEnvironment* penv = pdevice->getGUIEnvironment();

	// XML loading
	irr::io::IXMLReaderUTF8* xml = 
		pdevice->getFileSystem()->createXMLReaderUTF8(
			getPathOfMedia(xmlFile.c_str(), "xml").c_str());
	if (!xml) {
		std::ostringstream oss;
		oss << "ERROR : [" << xmlFile << "] not found!";
		throw std::runtime_error(oss.str());
	}
	xml_menu xm(xml, pdevice);
	xm.registerState(this);

	// change the global transparency settings
	for (irr::u32 i = 0; i < irr::gui::EGDC_COUNT ; ++i) {
		irr::video::SColor col = penv->getSkin()->getColor(
			(irr::gui::EGUI_DEFAULT_COLOR)i);
		double value =  
			parameter_set::instance()->getValueDouble("biolite.gui.alpha");
		value *= 255.0;
		col.setAlpha((irr::u32)value);
		penv->getSkin()->setColor((irr::gui::EGUI_DEFAULT_COLOR)i, col);
	}

	// init the plant
	plant::init(pdevice);

	// start the menu
	parameter_set::instance()->setValue(
		std::string("biolite.menu.current"),
		parameter_set::instance()->getValue("biolite.menu.start"));
	parameter_set::instance()->addListener(this);
}

void game::operator()(
	const std::string& name, 
	const std::string& value)
{
	if (name == std::string("biolite.world.seed")) {
		std::string strName = value;
		if (m_pworld && m_pworld->m_planet) {
			m_pworld->m_planet->m_seed = value;
			long seed = hash(value);
			createWorld((seed) ? seed : 1);
		}
	}
	if (name == std::string("biolite.planet.file"))
		createWorld(value);
	if (name == std::string("biolite.font.small"))
		loadFont();
	if (name == std::string("biolite.font.medium"))
		loadFont();
	if (name == std::string("biolite.font.large"))
		loadFont();
	if (name == std::string("biolite.camera.arcball.mouse")) {
		if ((value == std::string("left")) && m_parc) {
			m_parc->setCaptureMouseEvent(irr::EMIE_LMOUSE_PRESSED_DOWN);
			m_parc->setFreeMouseEvent(irr::EMIE_LMOUSE_LEFT_UP);
		}
		if ((value == std::string("right")) && m_parc) {
			m_parc->setCaptureMouseEvent(irr::EMIE_RMOUSE_PRESSED_DOWN);
			m_parc->setFreeMouseEvent(irr::EMIE_RMOUSE_LEFT_UP);
		}
	}
	if (name == std::string("biolite.camera")) {
		if (value == std::string("fixed")) {
			m_pdevice->getSceneManager()->setActiveCamera(m_camera);
		}
		if (value == std::string("arcball")) {
			m_pdevice->getSceneManager()->setActiveCamera(m_arc_cam);
		}
	}
}

void game::loadFont() {

	// get screen size
	irr::video::IVideoDriver* pvideo = m_pdevice->getVideoDriver();
	irr::core::dimension2d<irr::u32> screen_size = pvideo->getScreenSize();
	irr::gui::IGUIEnvironment* penv = m_pdevice->getGUIEnvironment();
	std::string strFont = "";
	if (screen_size.Height <= 600) {
		// load small font
		strFont = parameter_set::instance()->getValue("biolite.font.small");
	} else if (screen_size.Height <= 900) {
		// load medium font
		strFont = parameter_set::instance()->getValue("biolite.font.medium");
	} else {
		// load large font
		strFont = parameter_set::instance()->getValue("biolite.font.large");
	}
	if (strFont == std::string("<unknown>")) return;
	m_font = penv->getFont(getPathOfMedia(strFont.c_str(), "xml").c_str());
	if (!m_font) {
		std::ostringstream oss("");
		oss << "XML PARSE ERROR : invalid font : ";
		oss << strFont;
		throw std::runtime_error(oss.str().c_str());
	}
	penv->getSkin()->setFont(m_font);
}

void game::createWorld(const std::string& planet_file) {

	if (planet_file == std::string("<current>")) return;
	if (m_pworld) delete m_pworld;
	// create world
	m_pworld = new world(m_pdevice, planet_file);

	// create the camera
	irr::core::vector3df vEyePt(0.0f, 0.0f, -3.0f);
	irr::core::vector3df vLookatPt(0.0f, 0.0f, 0.0f);
	irr::f32 fov = (float)(M_PI / 4.0);
	irr::scene::ISceneManager* pscene = m_pdevice->getSceneManager();
	m_camera_node = pscene->addEmptySceneNode(m_pworld->m_planet_node);
	m_camera = pscene->addCameraSceneNode(m_camera_node, vEyePt, vLookatPt);
	m_camera->setFOV(fov);
	m_arc_cam = pscene->addCameraSceneNode(0, vEyePt, vLookatPt);
	m_arc_cam->setFOV(fov);
	irr::scene::ISceneNodeAnimator* anim;
	if (!m_parc) {
		irr::scene::CSceneNodeAnimatorCameraArcball* arc = 
			new irr::scene::CSceneNodeAnimatorCameraArcball(
				m_pdevice->getCursorControl(),
				m_arc_cam);
#ifdef _IRR_OSX_PLATFORM_
		if (parameter_set::instance()->getValue(
				std::string("biolite.device.fullscreen")) ==
			std::string("true"))
			arc->invertMouse(false);
#endif
		m_parc = 
			dynamic_cast<irr::scene::ISceneNodeAnimatorCameraArcball*>(arc);
		anim = dynamic_cast<irr::scene::ISceneNodeAnimator*>(m_parc);
		m_arc_cam->addAnimator(anim);
	}
	pscene->setActiveCamera(m_camera);
	m_camera->bindTargetAndRotation(true);
	m_camera_anim = pscene->createRotationAnimator(irr::core::vector3df(
		0.0f, 
		0.025f, 
		0.0f));
	// corect aspect ratio
	irr::core::dimension2d<irr::u32> screen_dim = 
		m_pdevice->getVideoDriver()->getCurrentRenderTargetSize();
	float aspect = (float)screen_dim.Width / (float)screen_dim.Height;
	m_camera->setAspectRatio(aspect);
	m_arc_cam->setAspectRatio(aspect);
	m_camera_node->addAnimator(m_camera_anim);
	parameter_set::instance()->setValue(
		"biolite.world.seed", 
		m_pworld->m_planet->m_seed);
}

void game::createWorld(unsigned long seed) {
	if (m_pworld) m_pworld->set_seed(seed);
	else throw std::runtime_error(
		"ERROR : game try to create a world with a seed");
}

game::~game() {
	parameter_set::instance()->removeListener(this);
	sound::release();
	if (m_pworld) delete m_pworld;
	if (m_parc) m_parc->drop();
}

bool game::OnEvent(const irr::SEvent& event) {
	return getCurrentState()->OnEvent(this, event);
}

void game::render(irr::IrrlichtDevice* pdevice) {
	if (!pdevice) return;
	m_pdevice = pdevice;
	// render
	if (m_pworld)
		m_pworld->render(pdevice);
	getCurrentState()->render(this, pdevice);
	sound::instance()->update();
}

void game::registerState(game_state* state) {
	m_registered_state.insert(make_pair(state->getName(), state));
}

void game::changeState(const std::string& name) {
	parameter_set::instance()->setValue(
		std::string("biolite.menu.current"),
		name);
	if (name == parameter_set::instance()->getValue("biolite.menu.end")) 
		exit(0);
	if (m_registered_state.find(name) == m_registered_state.end()) {
		std::ostringstream oss("");
		oss << "ERROR : no such a state : " << name;
		throw std::runtime_error(oss.str().c_str());
	}
	m_pstate = m_registered_state.find(name)->second;
}

game_state* game::getCurrentState() {
	changeState(
		parameter_set::instance()->getValue(
			std::string("biolite.menu.current")));
	return m_pstate;
}
