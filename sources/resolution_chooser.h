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

#ifndef BIOLITE_RESOLUTION_CHOOSER_HEADER_DEFINED
#define BIOLITE_RESOLUTION_CHOOSER_HEADER_DEFINED

#include "main.h"

class irr_resolution_chooser : public irr::IEventReceiver {
protected :
	std::list< std::pair<std::wstring, irr::core::dimension2du> > m_resolutions;
	std::list< std::pair<std::wstring, irr::video::E_DRIVER_TYPE> > m_drivers;
	std::list< std::pair<std::wstring, irr::s32> > m_depths;
	bool m_is_ok;
	bool m_is_cancel;
	irr::s32 m_ok_id;
	irr::s32 m_cancel_id;
	irr::gui::IGUIButton* m_ok_gui;
	irr::gui::IGUIButton* m_cancel_gui;
	irr::gui::IGUIComboBox* m_resolution_gui;
	irr::gui::IGUIComboBox* m_depth_gui;
	irr::gui::IGUIComboBox* m_driver_gui;
	irr::gui::IGUICheckBox* m_fullscreen_gui;
	irr::video::ITexture* m_back_tex;
	irr::IrrlichtDevice* m_pdevice;
public :
	//! constructor need a valid device (with some drawing abilities)
	irr_resolution_chooser(irr::IrrlichtDevice* pdevice);
	virtual ~irr_resolution_chooser();
	//! return false if finished
	bool runOnce(irr::IrrlichtDevice* pdevice);
	//! event handling callback
	virtual bool OnEvent(const irr::SEvent& event);
	//! return the parameter for the driver creation
	irr::SIrrlichtCreationParameters getParameters() const;
	//! was ok button pushed?
	bool ok() const { return m_is_ok; }
	//! was cancel button pushed?
	bool cancel() const { return m_is_cancel; }
};

#endif // BIOLITE_RESOLUTION_CHOOSER_HEADER_DEFINED
