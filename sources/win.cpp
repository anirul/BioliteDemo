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
#include "win.h"

irr_win::irr_win(irr::IrrlichtDevice* pdevice) {
	m_pgame = new game(pdevice);
	m_lasttimer = pdevice->getTimer()->getTime();
}

irr_win::~irr_win() {
	if (m_pgame) 
		delete m_pgame;
	m_pgame = NULL;
}

bool irr_win::runOnce(irr::IrrlichtDevice* pdevice) {
	pdevice->setEventReceiver(this);
	if (pdevice->run()) {
		irr::video::IVideoDriver* pvideo = pdevice->getVideoDriver();
		if (pdevice->isWindowActive()) {
			irr::scene::ISceneManager* pscene = pdevice->getSceneManager();
			if (pvideo && pvideo->beginScene()) {
				pscene->drawAll();
				m_pgame->render(pdevice);
				pdevice->getGUIEnvironment()->drawAll();
				pvideo->endScene();
			}
		} 
		{
			std::wstringstream wss(L"");
			wss << L"Biolite - Irrlicht Version - [" 
				<< pvideo->getName()
				<< L" FPS:" 
				<< pvideo->getFPS() 
				<< L" MTri/sec:" 
				<< pvideo->getPrimitiveCountDrawn(1) * (1.f / 1000000.f)
				<< L"]";
			pdevice->setWindowCaption(wss.str().c_str());
		}
		pdevice->yield();
		return true;
	}
	pdevice->getGUIEnvironment()->clear();
	return false;
}

bool irr_win::OnEvent(const irr::SEvent& event) {
	switch (event.EventType) {
		case irr::EET_GUI_EVENT :
		case irr::EET_JOYSTICK_INPUT_EVENT :
		case irr::EET_KEY_INPUT_EVENT :
		case irr::EET_MOUSE_INPUT_EVENT :
			return m_pgame->OnEvent(event);
		case irr::EET_USER_EVENT :
			return OnUserEvent(event.UserEvent);
		case irr::EET_LOG_TEXT_EVENT :
			return OnLogEvent(event.LogEvent);
		default :
			break;
	}
	return false;
}

bool irr_win::OnLogEvent(const irr::SEvent::SLogEvent& event) {
	std::string strLevel;
	switch (event.Level) {
		case irr::ELL_INFORMATION :
			strLevel = "INFORMATION";
			break;
		case irr::ELL_WARNING :
			strLevel = "WARNING";
			break;
		case irr::ELL_ERROR :
			strLevel = "ERROR";
			break;
		case irr::ELL_NONE :
			strLevel = "NONE";
			break;
		default :
			strLevel = "UNKNOWN";
			break;
	}
	std::cout << "LOG[" << strLevel << "] " << event.Text << std::endl;
	return true;
}

bool irr_win::OnUserEvent(const irr::SEvent::SUserEvent& event) {
	return false;
}

