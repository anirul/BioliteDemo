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

sound::sound(irr::IrrlichtDevice* device) :
#ifdef USE_IRRKLANG
m_engine(NULL), m_sound_map(), m_music(NULL),
#endif // USE_IRRKLANG
#ifdef USE_GORILLA
m_mgr(nullptr),
m_mixer(nullptr),
m_stream_mgr(nullptr),
#endif // USE_GORILLA
m_device(device),
m_current_music("")
{
	if (!m_device)
		throw std::runtime_error("undefined device!");
#ifdef USE_IRRKLANG
	m_engine = irrklang::createIrrKlangDevice();
#endif // USE_IRRKLANG
#ifdef USE_GORILLA
	gc_initialize(0);
	m_mgr = gau_manager_create();
	m_mixer = gau_manager_mixer(m_mgr);
	m_stream_mgr = gau_manager_streamManager(m_mgr);
#endif // USE_GORILLA
	parameter_set::instance()->addListener(this);
}

sound* sound::instance(irr::IrrlichtDevice* device) {
	if (!m_instance)
		m_instance = new sound(device);
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
#ifdef USE_GORILLA
	gau_manager_destroy(m_mgr);
	gc_shutdown();
#endif // USE_GORILLA
}

#ifdef USE_GORILLA
std::string sound::getExtention(const std::string& file) const {
	return file.substr(file.find_last_of(".") + 1);
}

bool sound::isStreamFile(const std::string& file) const {
	std::string ext = getExtention(file);
	if (ext == std::string("wav")) return false;
	if (ext == std::string("ogg")) return true;
	throw std::runtime_error(std::string("unknown extention : ") + ext);
}
#endif // USE_GORILLA

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
#ifdef USE_GORILLA
			stop(m_current_music);
			play(value, true);
#endif // USE_GORILLA
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
#ifdef USE_GORILLA
	if (m_files.find(name) == m_files.end())
		return false;
#endif // USE_GORILLA
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
#ifdef USE_GORILLA
	m_files.insert(std::make_pair(name, file));
#endif // USE_GORILLA
}

#ifdef USE_GORILLA
ga_Handle* sound::getHandle(const std::string& name) {
	auto it = m_files.find(name);
	if (it == m_files.end()) {
		std::ostringstream oss;
		oss << "could not find file named : " << name;
		throw std::runtime_error(oss.str());
	}
	auto file = it->second;
	std::string path = getPathOfMedia(file.c_str());
	std::string ext = getExtention(path);
	{
		if (!m_device->getFileSystem()->existFile(path.c_str())) {
			throw std::runtime_error("file don't exist : " + path);
		}
		auto* irrfile = m_device->getFileSystem()->createAndOpenFile(path.c_str());
		std::string temp;
		temp.resize(irrfile->getSize());
		irrfile->read(&temp[0], irrfile->getSize());
		m_memory.insert(std::make_pair(name, ga_memory_create(&temp[0],
															  temp.size())));
	}
	ga_Handle* handle = nullptr;
	ga_DataSource* data_source = gau_data_source_create_memory(m_memory[name]);
	handle = gau_create_handle_buffered_data(m_mixer,
											 m_stream_mgr,
											 data_source,
											 ext.c_str(),
											 &gau_on_finish_destroy,
											 nullptr,
											 nullptr);
	if (!handle) {
		std::stringstream ss("");
		ss << "unable to load sound : " << path;
		throw std::runtime_error(ss.str());
	}
	m_buffers.insert(std::make_pair(name, handle));
	return handle;
}
#endif // USE_GORILLA

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
void sound::play(const std::string& name, bool loop) {
#ifdef USE_GORILLA
	if (ga_handle_play(getHandle(name)) != GC_SUCCESS) {
		std::ostringstream oss;
		oss << "error playing sound : " << name;
		throw std::runtime_error(oss.str());
	}
#endif // USE_GORILLA
}
#endif // USE_IRRKLANG

void sound::stop(const std::string& name) {
#ifdef USE_GORILLA
	auto it = m_buffers.find(name);
	if (it == m_buffers.end()) {
		std::stringstream ss("");
		ss << "unknown sound : " << name;
		throw std::runtime_error(ss.str());
	}
	if (ga_handle_stop(it->second) != GC_SUCCESS) {
		std::ostringstream oss;
		oss << "error stopping sound : " << name;
		throw std::runtime_error(oss.str());
	}
#endif // USE_GORILLA
}

void sound::update() {
#ifdef USE_IRRKLANG
	m_engine->update();
#endif // USE_IRRKLANG
#ifdef USE_GORILLA
	gau_manager_update(m_mgr);
#endif // USE_GORILLA
}
