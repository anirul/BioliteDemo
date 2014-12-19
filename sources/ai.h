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

#ifndef BIOLITE_AI_HEADER_DEFINED
#define BIOLITE_AI_HEADER_DEFINED

#include "world.h"

class game_logic;

class ai {
protected :
	world* m_pw;
	game_logic* m_gl;
	bool m_had_plant[MAX_CLIENT];
public :
	ai(game_logic* gl, world* pw) : m_pw(pw), m_gl(gl) {
		for (int i = 0; i < MAX_CLIENT; ++i)
			m_had_plant[i] = false;
	}
	virtual ~ai() {}
	virtual void tick(int player_id, int plant_count) = 0;
};

class easy_ai : public ai {
public :
	easy_ai(game_logic* gl, world* pw) : ai(gl, pw) {}
	virtual ~easy_ai() {}
	virtual void tick(int player_id, int plant_count);
};

class medium_ai : public ai {
public :
	medium_ai(game_logic* gl, world* pw) : ai(gl, pw) {}
	virtual ~medium_ai() {}
	virtual void tick(int player_id, int plant_count);
};

class hard_ai : public ai {
public :
	hard_ai(game_logic* gl, world* pw) : ai(gl, pw) {}
	virtual ~hard_ai() {}
	virtual void tick(int player_id, int plant_count);
};

#endif // BIOLITE_AI_HEADER_DEFINED