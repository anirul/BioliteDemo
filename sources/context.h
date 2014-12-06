/*
 * Copyright (c) 2014, anirul
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

#ifndef FATTYCURD_CONTEXT_HEADER_DEFINED
#define FATTYCURD_CONTEXT_HEADER_DEFINED

#include <string>
#include <map>
#include <irrlicht.h>

const std::map<std::string, irr::video::E_DRIVER_TYPE> c_name_driver {
	{ "null", irr::video::EDT_NULL },
	{ "software", irr::video::EDT_SOFTWARE },
	{ "burning", irr::video::EDT_BURNINGSVIDEO },
	{ "d3d8", irr::video::EDT_DIRECT3D8 },
	{ "d3d9", irr::video::EDT_DIRECT3D9 },
	{ "opengl", irr::video::EDT_OPENGL },
};

const std::map<irr::video::E_DRIVER_TYPE, std::string> c_driver_name {
	{ irr::video::EDT_NULL, "null" },
	{ irr::video::EDT_SOFTWARE, "software" },
	{ irr::video::EDT_BURNINGSVIDEO, "burning" },
	{ irr::video::EDT_DIRECT3D8, "d3d8" },
	{ irr::video::EDT_DIRECT3D9, "d3d9" },
	{ irr::video::EDT_OPENGL, "opengl" },
};

const std::map<std::string, irr::ELOG_LEVEL> c_name_log {
	{ "debug", irr::ELL_DEBUG },
	{ "error", irr::ELL_ERROR },
	{ "information", irr::ELL_INFORMATION },
	{ "none", irr::ELL_NONE },
	{ "warning", irr::ELL_WARNING },
};

const std::map<irr::ELOG_LEVEL, std::string> c_log_name {
	{ irr::ELL_DEBUG, "debug" },
	{ irr::ELL_ERROR, "error" },
	{ irr::ELL_INFORMATION, "information" },
	{ irr::ELL_NONE, "none" },
	{ irr::ELL_WARNING, "warning" },
};

class context {
public:
	context(irr::IEventReceiver* callback = nullptr);
	~context();
public:
	irr::IrrlichtDevice* m_device;
	irr::IEventReceiver* m_callback;
	irr::SIrrlichtCreationParameters m_params;
	irr::s32 m_tileMaterial;
	irr::s32 m_characterMaterial;
public:
	void loadConfigFromFile(const std::string& file);
	void saveConfigToFile(const std::string& file);
public:
	void createDefaultDevice();
	void createNullDevice();
	void createFromParamsDevice(const irr::SIrrlichtCreationParameters& params);
	void createFromXML();
public:
	void fadeIn(const irr::video::SColor& c, irr::u32 ms);
	void fadeOut(const irr::video::SColor& c, irr::u32 ms);
};


#endif // FATTYCURD_CONTEXT_HEADER_DEFINED