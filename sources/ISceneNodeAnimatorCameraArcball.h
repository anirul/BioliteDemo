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

#ifndef ISCENENODEANIMATORCAMERAARCBALL_H_
#define ISCENENODEANIMATORCAMERAARCBALL_H_

namespace irr {
	namespace scene {

		class ISceneNodeAnimatorCameraArcball : public ISceneNodeAnimator {
		public:
			virtual void captureMouse() = 0;
			virtual bool isMouseCaptured() const = 0;
			virtual void freeMouse() = 0;
			virtual void setCaptureMouseEvent(EMOUSE_INPUT_EVENT event) = 0;
			virtual EMOUSE_INPUT_EVENT getCaptureMouseEvent() const = 0;
			virtual void setFreeMouseEvent(EMOUSE_INPUT_EVENT event) = 0;
			virtual EMOUSE_INPUT_EVENT getFreeMouseEvent() const = 0;
		};

	} // end scene
} // end irr

#endif /* ISCENENODEANIMATORCAMERAARCBALL_H_ */
