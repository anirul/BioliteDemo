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
#include "resolution_chooser.h"
#include "media_collection.h"
#include "persistant_set.h"
#include "parameter_set.h"

// comparison, not case sensitive.
bool compare_resolution(
	std::pair<std::wstring, irr::core::dimension2du> first, 
	std::pair<std::wstring, irr::core::dimension2du> second)
{
	return (first.second.getArea() < second.second.getArea());
}

bool compare_depth(
	std::pair<std::wstring, irr::s32> first, 
	std::pair<std::wstring, irr::s32> second)
{
	return (first.second < second.second);
}

irr_resolution_chooser::irr_resolution_chooser(irr::IrrlichtDevice* pdevice) 
	:	m_is_ok(false),
	 	m_is_cancel(false),
	 	m_ok_id(101),
	 	m_cancel_id(102),
	 	m_ok_gui(NULL),
	 	m_cancel_gui(NULL),
		m_resolution_gui(NULL),
	 	m_depth_gui(NULL),
	 	m_driver_gui(NULL),
	 	m_fullscreen_gui(NULL),
	 	m_back_tex(NULL),
	 	m_pdevice(pdevice)
{
	irr::video::IVideoModeList* pmode = pdevice->getVideoModeList();
	for (int i = 0; i < pmode->getVideoModeCount(); ++i) {
		irr::core::dimension2du resolution = pmode->getVideoModeResolution(i);
		irr::s32 depth = pmode->getVideoModeDepth(i);
		{ //! add resolution
			std::wstringstream wss(L"");
			wss << resolution.Width << L"x" << resolution.Height;
			m_resolutions.push_back(make_pair(wss.str(), resolution));
		}
		{ //! add depth
			std::wstringstream wss(L"");
			wss << depth;
			m_depths.push_back(make_pair(wss.str(), depth));
		}
	}
	m_resolutions.sort(compare_resolution);
	m_resolutions.unique();
	
	m_depths.sort(compare_depth);
	m_depths.unique();
	
#ifdef _IRR_WINDOWS_
	m_drivers.push_back(make_pair(std::wstring(L"Direct3D8"), irr::video::EDT_DIRECT3D8));
	m_drivers.push_back(make_pair(std::wstring(L"Direct3D9"), irr::video::EDT_DIRECT3D9));
#endif
	m_drivers.push_back(make_pair(std::wstring(L"OpenGL"), irr::video::EDT_OPENGL));

	
	irr::gui::IGUIEnvironment* penv = pdevice->getGUIEnvironment();
	irr::gui::IGUIFont* font = penv->getFont(
		getPathOfMedia("Arial12.xml", "xml").c_str());
	if (!font) throw std::runtime_error("ERROR : Could not load font");
	penv->getSkin()->setFont(font, irr::gui::EGDF_DEFAULT);
	irr::video::IImage* pimage =
		pdevice->getVideoDriver()->createImageFromFile(
			getPathOfMedia("resolution_back.jpg", "jpg").c_str());
	m_back_tex = pdevice->getVideoDriver()->addTexture(
		getPathOfMedia("resolution_back.jpg").c_str(), 
		pimage);
	penv->addImage(
		m_back_tex,
		irr::core::vector2di(0, 0));
	penv->addStaticText(
		L"Resolution",
		irr::core::recti(5, 5, 155, 35),
		false,
		false,
		penv->getRootGUIElement())->setOverrideColor(
			irr::video::SColor(0xffffffff));
	m_resolution_gui = penv->addComboBox(
		irr::core::recti(165, 5, 315, 35),
		penv->getRootGUIElement());
	penv->addStaticText(
		L"Depth",
		irr::core::recti(5, 45, 155, 75),
		false,
		false,
		penv->getRootGUIElement())->setOverrideColor(
			irr::video::SColor(0xffffffff));
	m_depth_gui = penv->addComboBox(
		irr::core::recti(165, 45, 315, 75),
		penv->getRootGUIElement());
	penv->addStaticText(
		L"Driver",
		irr::core::recti(5, 85, 155, 115),
		false,
		false,
		penv->getRootGUIElement())->setOverrideColor(
			irr::video::SColor(0xffffffff));
	m_driver_gui = penv->addComboBox(
		irr::core::recti(165, 85, 315, 115),
		penv->getRootGUIElement());
	penv->addStaticText(
		L"Windowed",
		irr::core::recti(5, 125, 155, 155),
		false,
		false,
		penv->getRootGUIElement())->setOverrideColor(
			irr::video::SColor(0xffffffff));
	m_fullscreen_gui = penv->addCheckBox(
		false,
		irr::core::recti(165, 125, 315, 155),
		penv->getRootGUIElement());
	m_ok_gui = penv->addButton(
		irr::core::recti(5, 165, 155, 195),
		penv->getRootGUIElement(),
		m_ok_id,
		L"Launch!");
	m_cancel_gui = penv->addButton(
		irr::core::recti(165, 165, 315, 195),
		penv->getRootGUIElement(),
		m_cancel_id,
		L"Cancel");
	{ // add the resolutions to the combobox
			  
		std::list<std::pair<std::wstring, irr::core::dimension2du> >::reverse_iterator ite;
		for (ite = m_resolutions.rbegin(); ite != m_resolutions.rend(); ++ite)
		{
			m_resolution_gui->addItem(ite->first.c_str());
		}
		m_resolution_gui->setSelected(0);

#ifdef _IRR_WINDOWS_
		// set as default the resolution saved in the registry
		std::wstring savedResolution = L"";
		savedResolution = persistant_set::instance()->getValue("Resolution");

		int i = 0;
		for (ite = m_resolutions.rbegin(); ite != m_resolutions.rend(); ++ite)
		{
            if (savedResolution == ite->first)
			{
                m_resolution_gui->setSelected(i);
			}
            i++;
		}
#endif
	}
	{ // add the depth to the combobox
		std::list< std::pair<std::wstring, irr::s32> >::reverse_iterator ite;
		for (ite = m_depths.rbegin(); ite != m_depths.rend(); ++ite)
			m_depth_gui->addItem(ite->first.c_str());
		m_depth_gui->setSelected(0);

#ifdef _IRR_WINDOWS_
		// set as default the resolution saved in the registry
		std::wstring savedDepth = L"";
		savedDepth = persistant_set::instance()->getValue("Depth");

		int i = 0;
		for (ite = m_depths.rbegin(); ite != m_depths.rend(); ++ite)
		{
            if (savedDepth == ite->first)
			{
                m_depth_gui->setSelected(i);
			}
            i++;
		}
#endif
	}

	{ // add drivers to the combobox
		std::list< std::pair<std::wstring, irr::video::E_DRIVER_TYPE> >::iterator ite;
		for (ite = m_drivers.begin(); ite != m_drivers.end(); ++ite)
			m_driver_gui->addItem(ite->first.c_str());
		m_driver_gui->setSelected(0);

#ifdef _IRR_WINDOWS_
		// set as default the resolution saved in the registry
		std::wstring savedDriver = L"";
		savedDriver = persistant_set::instance()->getValue("Driver");

		int i = 0;
		for (ite = m_drivers.begin(); ite != m_drivers.end(); ++ite)
		{
            if (savedDriver == ite->first)
			{
                m_driver_gui->setSelected(i);
			}
            i++;
		}
#endif
	}
}

irr_resolution_chooser::~irr_resolution_chooser() {}

bool irr_resolution_chooser::runOnce(irr::IrrlichtDevice* pdevice) {
	pdevice->setEventReceiver(this);
	if (pdevice->run()) {
		irr::video::IVideoDriver* pvideo = pdevice->getVideoDriver();
		if (pdevice->isWindowActive()) {
			irr::scene::ISceneManager* pscene = pdevice->getSceneManager();
			if (pvideo && pvideo->beginScene()) {
				pscene->drawAll();
				pdevice->getGUIEnvironment()->drawAll();
				pvideo->endScene();
			}
		}
		pdevice->yield();
		return true;
	}
	pdevice->getSceneManager()->clear();
	return false;
}

bool irr_resolution_chooser::OnEvent(const irr::SEvent& event) {
	if (event.EventType == irr::EET_GUI_EVENT) {
		switch (event.GUIEvent.EventType) {
		case irr::gui::EGET_BUTTON_CLICKED :
			if (event.GUIEvent.Caller->getID() == m_ok_id) {
//#ifdef _IRR_WINDOWS_
				std::wstring resolution = m_resolution_gui->getItem(m_resolution_gui->getSelected());
				std::wstring depth = m_depth_gui->getItem(m_depth_gui->getSelected());
				std::wstring driver = m_driver_gui->getItem(m_driver_gui->getSelected());
				persistant_set::instance()->setValue("Resolution", std::string(resolution.begin(), resolution.end()));
				persistant_set::instance()->setValue("Depth", std::string(depth.begin(), depth.end()));
				persistant_set::instance()->setValue("Driver", std::string(driver.begin(), driver.end()));
//#endif
				parameter_set::instance()->setValue(
					std::string("biolite.device.resolution"),
					std::string(resolution.begin(), resolution.end()));
				parameter_set::instance()->setValue(
					std::string("biolite.device.depth"),
					std::string(depth.begin(), depth.end()));
				parameter_set::instance()->setValue(
					std::string("biolite.device.driver"),
					std::string(driver.begin(), driver.end()));
				m_is_ok = true;
				if (m_pdevice) m_pdevice->closeDevice();
				return true;
			}
			if (event.GUIEvent.Caller->getID() == m_cancel_id) {
				m_is_cancel = true;
				if (m_pdevice) m_pdevice->closeDevice();
				return true;
			}
			return false;
		default :
			return false;
		}
	}
	if (event.EventType == irr::EET_LOG_TEXT_EVENT) {
		std::cout << event.LogEvent.Text << std::endl;
	}
	return false;
}

irr::SIrrlichtCreationParameters irr_resolution_chooser::getParameters() const {
	irr::SIrrlichtCreationParameters params;
	std::wstring resolution = m_resolution_gui->getItem(m_resolution_gui->getSelected());
	std::wstring depth = m_depth_gui->getItem(m_depth_gui->getSelected());
	std::wstring driver = m_driver_gui->getItem(m_driver_gui->getSelected());
	//params.WindowSize = (m_resolutions.find(resolution))->second;
	{
		std::list< std::pair<std::wstring, irr::core::dimension2du> >::const_iterator ite;
		for (ite = m_resolutions.begin(); ite->first != resolution; ++ite);
		params.WindowSize = ite->second;
	}
	//params.Bits = (m_depths.find(depth))->second;
	{
		std::list< std::pair<std::wstring, irr::s32> >::const_iterator ite;
		for (ite = m_depths.begin(); ite->first != depth; ++ite);
		params.Bits = ite->second;
	}
	
	//params.DriverType = (m_drivers.find(driver))->second;
	{
		std::list< std::pair<std::wstring, irr::video::E_DRIVER_TYPE> >::const_iterator ite;
		for (ite = m_drivers.begin(); ite->first != driver; ++ite);
		params.DriverType = ite->second;
	}
	parameter_set::instance()->setValue(
		std::string("biolite.device.fullscreen"),
		(!m_fullscreen_gui->isChecked()) ?
			std::string("true") :
			std::string("false"));
	params.Fullscreen = !m_fullscreen_gui->isChecked();
	params.ZBufferBits = 32;
	params.Stencilbuffer = true;
	params.Vsync = true;
	params.Doublebuffer = true;
	return params;
}
