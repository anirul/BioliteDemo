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

#ifndef CSCENENODEANIMATORCAMERAARCBALL_H_
#define CSCENENODEANIMATORCAMERAARCBALL_H_

#include <irrlicht.h>
#include "ISceneNodeAnimatorCameraArcball.h"

namespace irr {
	namespace scene {

		class CSceneNodeAnimatorCameraArcball : 
			public ISceneNodeAnimatorCameraArcball {
		public:
			//! Constructor
			CSceneNodeAnimatorCameraArcball(
				gui::ICursorControl* cursorControl,
				ICameraSceneNode* camera,
				EMOUSE_INPUT_EVENT captureEvent = EMIE_LMOUSE_PRESSED_DOWN,
				EMOUSE_INPUT_EVENT freeEvent = EMIE_LMOUSE_LEFT_UP,
				float zoomMax = 0.5,
				float zoomMin = 1.5,
				float zoomStep = 0.1);

			//! Destructor
			virtual ~CSceneNodeAnimatorCameraArcball();

			virtual void invertMouse(bool invert) {
				InvertAxes = invert;
			}

			//! Capture the mouse
			virtual void captureMouse();

			//! tell if the mouse is captured
			virtual bool isMouseCaptured() const { return mouseDrag; }

			//! Free the mouse
			virtual void freeMouse();

			//! set the capture event for the mouse
			virtual void setCaptureMouseEvent(EMOUSE_INPUT_EVENT event)
				{ CaptureMouseEvent = event; }

			//! get the capture event of the mouse
			virtual EMOUSE_INPUT_EVENT getCaptureMouseEvent() const
				{ return CaptureMouseEvent; }

			//! set the free event for the mouse
			virtual void setFreeMouseEvent(EMOUSE_INPUT_EVENT event)
				{ FreeMouseEvent = event; }

			//! get the free event for the mouse
			virtual EMOUSE_INPUT_EVENT getFreeMouseEvent() const
				{ return FreeMouseEvent; }

			//! Animate the scene node, currently only work on cameras
			virtual void animateNode(ISceneNode* node, u32 timeMs);

			//! Event receiver
			virtual bool OnEvent(const SEvent& event);

			//! This animator will receive events when attached to the active camera
			virtual bool isEventReceiverEnabled() const
				{ return true; }

			//! Returns the type of this animator
			virtual ESCENE_NODE_ANIMATOR_TYPE getType() const
				{ return ESNAT_CAMERA_FPS; }

			//! create clone of this animator
			virtual ISceneNodeAnimator* createClone(
				ISceneNode* node,
				ISceneManager* newManager = 0);
		private :
			core::vector3df position2vector(const core::position2df& pos) const;
			gui::ICursorControl* CursorControl;
			core::position2df CursorDown;
			core::quaternion QuatOri;
			core::vector3df CamOri;
			core::vector3df CamUp;
			f32 CamFOV;
			ICameraSceneNode* Camera;
			bool InvertAxes;
			bool mouseDrag;
			float maxZoom;
			float minZoom;
			float stepZoom;
			float actualZoom;
			EMOUSE_INPUT_EVENT CaptureMouseEvent;
			EMOUSE_INPUT_EVENT FreeMouseEvent;
		};

	}
}

#endif /* CSCENENODEANIMATORCAMERAARCBALL_H_ */
