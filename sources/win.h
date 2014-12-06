/*
 * Copyright (c) 2006-2014, anirul
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

#ifndef FATTYCURD_WIN_HEADER_DEFINED
#define FATTYCURD_WIN_HEADER_DEFINED

class game;
class context;

class irr_win : public irr::IEventReceiver  {
	public :
	irr_win(const std::string& xml);
	virtual ~irr_win();
	bool runOnce(irr::IrrlichtDevice* pdevice);
	virtual bool OnEvent(const irr::SEvent& event);
	context* getContext() { return m_context; }
	protected :
	bool OnGUIEvent(const irr::SEvent::SGUIEvent& event);
	bool OnJoystickEvent(const irr::SEvent::SJoystickEvent& event);
	bool OnKeyInput(const irr::SEvent::SKeyInput& event);
	bool OnLogEvent(const irr::SEvent::SLogEvent& event);
	bool OnMouseInput(const irr::SEvent::SMouseInput& event);
	bool OnUserEvent(const irr::SEvent::SUserEvent& event);
private:
	long m_lasttimer;
	game* m_pgame;
	context* m_context;
	irr::s32 m_blurH;
	irr::s32 m_blurV;
	irr::s32 m_SSAO;
	irr::s32 m_SSAOCombine;
	irr::s32 m_bloomP;
	irr::s32 m_brightPass;
};


#endif // FATTYCURD_WIN_HEADER_DEFINED

