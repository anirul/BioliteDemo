/*
 * Copyright (c) 2010, anirul
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

#ifndef __C_GUI_COLOR_PICK_DIALOG_H_INCLUDED__
#define __C_GUI_COLOR_PICK_DIALOG_H_INCLUDED__

//#include "IrrCompileConfig.h"
//#ifdef _IRR_COMPILE_WITH_GUI_

#include "IGUIColorPickDialog.h"
#include "IGUIButton.h"
#include "IGUIEditBox.h"
#include "IGUIScrollBar.h"
#include "IGUIImage.h"
#include "irrArray.h"


namespace irr
{
namespace gui
{

	class CGUIColorPickDialog : public IGUIColorPickDialog
	{
	public:
	
		//! constructor
		CGUIColorPickDialog(
			const wchar_t* title, 
			const video::SColor& start, 
			IGUIEnvironment* environment, 
			IGUIElement* parent, 
			s32 id);
		
		//! destructor
		virtual ~CGUIColorPickDialog();
		
		//! called if an event happened.
		virtual bool OnEvent(const SEvent& event);

		//! draws the element and its children
		virtual void draw();

		//! return the color selected (full transparent black if none was selected)
		virtual const video::SColor getColor() const;

		//! set the color (used internally but also to set default color).
		virtual void setColor(const video::SColor& col);

	private:
	
		video::ITexture* createColorDisplay(
			const core::dimension2d<u32>& dim, 
			const video::SColor& col);
		void sendCancelEvent();
		void sendSelectedEvent();

		struct SBatteryItem
		{
			f32 Incoming;
			f32 Outgoing;
			IGUIEditBox * Edit;
			IGUIScrollBar *Scrollbar;
		};

		core::array< SBatteryItem > Battery;
		gui::IGUIImage* ColorImage;
		video::ITexture* ColorDisplay;
		video::SColor Color;
		core::position2d<s32> DragStart;
		bool Dragging;
		IGUIButton* CloseButton;
		IGUIButton* OKButton;
		IGUIButton* CancelButton;
	};

} // end namespace gui
} // end namespace irr

// #endif // _IRR_COMPILE_WITH_GUI_

#endif // __C_GUI_COLOR_SELECT_DIALOG_H_INCLUDED__

