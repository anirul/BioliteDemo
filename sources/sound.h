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

#ifndef BIOLITE_SOUND_HEADER_DEFINED
#define BIOLITE_SOUND_HEADER_DEFINED

#include <map>
#include <string>
#ifdef USE_IRRKLANG
#include <irrKlang.h>
#endif // USE_IRRKLANG
#include "parameter_set.h"

class sound : public parameter_set_listener {
protected :
	static sound* m_instance;
	sound();
#ifdef USE_IRRKLANG
	irrklang::ISoundEngine* m_engine;
	std::map<std::string, irrklang::ISoundSource*> m_sound_map;
	irrklang::ISound* m_music;
#endif // USE_IRRKLANG
	std::string m_current_music;
public :
	//! create and release sound instance
	static sound* instance();
	static void release();
	//! dtor
	virtual ~sound();
	//! from parameter_set_listener
	virtual void operator()(const std::string& name, const std::string& value);
	//! add a sound
	void load(const std::string& file, const std::string& name);
	//! check if sound loaded
	bool isLoaded(const std::string& name);
	//! play a sound
#ifdef USE_IRRKLANG
	irrklang::ISound* play(const std::string& name, bool loop = false);
#else
	void* play(const std::string& name, bool loop = false);
#endif // USE_IRRKLANG
	//! stop a sound
	void stop(const std::string& name);
	//! update the sound engine needed?
	void update();
};

#endif // BIOLITE_SOUND_HEADER_DEFINED