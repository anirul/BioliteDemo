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

#include "sound.h"
#include "media_collection.h"

sound* sound::m_instance = NULL;

sound::sound() :
#ifdef USE_IRRKLANG
	m_engine(NULL), m_sound_map(), m_music(NULL),
#endif // USE_IRRKLANG
	m_current_music("")
{
#ifdef USE_IRRKLANG
	m_engine = irrklang::createIrrKlangDevice();
#endif // USE_IRRKLANG
	parameter_set::instance()->addListener(this);
}

sound* sound::instance() {
	if (!m_instance)
		m_instance = new sound();
	return m_instance;
}

void sound::release() {
	if (m_instance)
		delete m_instance;
	m_instance = NULL;
}

sound::~sound() {
	parameter_set::instance()->removeListener(this);
#ifdef USE_IRRKLANG
	if (m_engine)
		m_engine->drop();
	m_engine = NULL;
#endif // USE_IRRKLANG
}

void sound::operator()(const std::string& name, const std::string& value) {
	if (name == std::string("biolite.music.title")) {
		load(value, "title");
	}
	if (name == std::string("biolite.music.play")) {
		load(value, "play");
	}
	if (name == std::string("biolite.music.edit")) {
		load(value, "edit");
	}
	if (name == std::string("biolite.music.current")) {
		if (value != m_current_music) {
#ifdef USE_IRRKLANG
			if (m_music) m_music->stop();
			m_music = play(value, true);
#endif // USE_IRRKLANG
			m_current_music = value;
		}
	}
	if (name.find(std::string("biolite.sound.")) != std::string::npos) {
		std::string sound_name = name.substr(14, name.length());
		if (!isLoaded(sound_name))
			load(value, sound_name);
	}
}

bool sound::isLoaded(const std::string& name) {
#ifdef USE_IRRKLANG
	if (m_sound_map.find(name) == m_sound_map.end()) 
		return false;
#endif // USE_IRRKLANG
	return true;
}

void sound::load(const std::string& file, const std::string& name) {
#ifdef USE_IRRKLANG
	irrklang::ISoundSource* source = m_engine->addSoundSourceFromFile(
		getPathOfMedia(file.c_str()).c_str());
	if (source == NULL) {
		std::string error("ERROR : could not load file : ");
		error.append(file);
		throw std::runtime_error(error);
	}
	m_sound_map.insert(std::make_pair(name, source));
#endif // USE_IRRKLANG
}

#ifdef USE_IRRKLANG
irrklang::ISound* sound::play(const std::string& name, bool loop) {
	std::map<std::string, irrklang::ISoundSource*>::iterator ite;
	ite = m_sound_map.find(name);
	if (ite == m_sound_map.end()) {
		std::string error("ERROR : Play unknown sound : ");
		error.append(name);
		throw std::runtime_error(error);
	}
	return m_engine->play2D(ite->second, loop, false, true);
}
#else
void* sound::play(const std::string& name, bool loop) {
	return nullptr;
}
#endif // USE_IRRKLANG

void sound::update() {
#ifdef USE_IRRKLANG
	m_engine->update();
#endif // USE_IRRKLANG
}