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

#include <cmath>
#include "CSceneNodeAnimatorCameraArcball.h"
#include <iostream>

irr::scene::CSceneNodeAnimatorCameraArcball::CSceneNodeAnimatorCameraArcball(
	gui::ICursorControl* cursorControl,
	ICameraSceneNode* camera,
	EMOUSE_INPUT_EVENT captureEvent,
	EMOUSE_INPUT_EVENT freeEvent,
	float zoomMax,
	float zoomMin,
	float zoomStep)
	:	CursorControl(cursorControl),
	 	Camera(camera),
	 	InvertAxes(true),
	 	mouseDrag(false),
	 	maxZoom(zoomMax),
	 	minZoom(zoomMin),
	 	stepZoom(zoomStep),
	 	actualZoom(1.0),
	 	CaptureMouseEvent(captureEvent),
	 	FreeMouseEvent(freeEvent)
{
	CamOri = Camera->getPosition() - Camera->getTarget();
	CamUp = Camera->getUpVector();
	CamFOV = Camera->getFOV();
	QuatOri.makeIdentity();
}

irr::scene::CSceneNodeAnimatorCameraArcball::~CSceneNodeAnimatorCameraArcball() {}

irr::core::vector3df irr::scene::CSceneNodeAnimatorCameraArcball::position2vector(
	const irr::core::position2df& pos) const
{
	float fx = -(0.5f - pos.X);
	float fy = ((InvertAxes)?1:-1) * (0.5f - pos.Y);

	float fz   = 0.0f;
	float mag = fx*fx + fy*fy;

	if( mag > 1.0f)
	{
		float scale = 1.0f / sqrtf(mag);
		fx *= scale;
		fy *= scale;
	}
	else
		fz = sqrtf( 1.0f - mag );

	return irr::core::vector3df(fx, fy, fz);
}

void irr::scene::CSceneNodeAnimatorCameraArcball::animateNode(ISceneNode *node, u32 timeMs)
{
	if (!node || node->getType() != ESNT_CAMERA)
		return;

	if (!Camera->isInputReceiverEnabled())
		return;

	ISceneManager* smgr = Camera->getSceneManager();
	if (smgr && smgr->getActiveCamera() != Camera)
		return;

	core::vector3df target = Camera->getTarget();
	core::vector3df pos; // = Camera->getPosition() - target;
	core::vector3df up; // = Camera->getUpVector();
	core::quaternion quat;
	quat.makeIdentity();

	// compute the new quaternion
	if (CursorControl && mouseDrag) {
		core::position2df CursorPos = CursorControl->getRelativePosition();
		core::vector3df from = position2vector(CursorDown);
		core::vector3df to = position2vector(CursorPos);
		quat.rotationFromTo(from, to);
		CursorDown = CursorPos;
	}

	// should solve some camera miss placement effect
	core::vector3df begin(0, 0, -1);
	core::quaternion quatAxe;
	quatAxe.rotationFromTo(CamOri, begin);

	// increment the movement quaternion
	QuatOri = quat * QuatOri;

	// move position into the origine Axes
	pos = quatAxe * CamOri;
	up = quatAxe * CamUp;

	// move position with the incremented quaternion
	pos = QuatOri * pos;
	up = QuatOri * up;

	// inverte the quatAxe
	quatAxe.makeInverse();

	// put it back into the position coordinates
	pos = quatAxe * pos;
	up = quatAxe * up;

	// move the center according to target
	Camera->setPosition(pos + target);
//	Camera->setUpVector(up);
//	Camera->setFOV(CamFOV * actualZoom);
}

void irr::scene::CSceneNodeAnimatorCameraArcball::captureMouse() {
	CursorDown = CursorControl->getRelativePosition();
	mouseDrag = true;
}

void irr::scene::CSceneNodeAnimatorCameraArcball::freeMouse() {
	mouseDrag = false;
}

bool irr::scene::CSceneNodeAnimatorCameraArcball::OnEvent(const irr::SEvent & event)
{
	if (event.EventType == EET_MOUSE_INPUT_EVENT) {
		if (event.MouseInput.Event == CaptureMouseEvent) {
			captureMouse();
			return true;
		}
		if (event.MouseInput.Event == FreeMouseEvent) {
			freeMouse();
			return true;
		}
		if (event.MouseInput.Event == EMIE_MOUSE_WHEEL) {
			actualZoom -= event.MouseInput.Wheel * stepZoom;
			if (actualZoom < maxZoom) actualZoom = maxZoom;
			if (actualZoom > minZoom) actualZoom = minZoom;
			return true;
		}
	}
	return false;
}

irr::scene::ISceneNodeAnimator* irr::scene::CSceneNodeAnimatorCameraArcball::createClone(
	ISceneNode *node,
	ISceneManager *newManager)
{
	CSceneNodeAnimatorCameraArcball* newAnimator =
		new CSceneNodeAnimatorCameraArcball(CursorControl, Camera);
	return newAnimator;
}


