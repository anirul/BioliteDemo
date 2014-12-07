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

#include "xml_set_value.h"

#include "context.h"
#include "parameter_set.h"
#include "media_collection.h"

context::context(irr::IEventReceiver* callback) :
m_device(nullptr),
m_callback(callback),
m_params(),
m_tileMaterial(-1),
m_characterMaterial(-1)
{
	m_params.DriverType = irr::video::EDT_OPENGL;
}

context::~context()
{
	if (m_device)
		m_device->drop();
}

void context::createDefaultDevice() {
	if (m_device)
		m_device->drop();
	m_device = nullptr;
	irr::SIrrlichtCreationParameters params;
	params.AntiAlias = 4;
	params.Bits = 32;
	params.DeviceType = irr::EIDT_BEST;
	params.DisplayAdapter = 0;
	params.Doublebuffer = true;
	params.DriverMultithreaded = false;
	params.DriverType = irr::video::EDT_OPENGL;
	params.EventReceiver = m_callback;
	params.Fullscreen = false;
	params.HandleSRGB = false;
	params.HighPrecisionFPU = true;
	params.IgnoreInput = false;
	params.LoggingLevel = irr::ELL_NONE;
	params.Stencilbuffer = true;
	params.Stereobuffer = false;
	params.UsePerformanceTimer = true;
	params.Vsync = true;
	params.WindowId = nullptr;
	params.WindowSize = irr::core::dimension2du(1280, 720);
	params.WithAlphaChannel = true;
	params.ZBufferBits = 32;
	m_device = irr::createDeviceEx(params);
	if (!m_device)
		createFromParamsDevice(m_params);
	else
		m_params = params;
}

void context::createNullDevice()
{
	m_device = irr::createDevice(irr::video::EDT_NULL);
}

void context::createFromXML()
{
	if (m_device) m_device->drop();
	parameter_set* pps = parameter_set::instance();
	m_device = nullptr;
	irr::SIrrlichtCreationParameters params;
	params.AntiAlias = (irr::u8)pps->getValueDouble("fattycurd.device.antialias");
	params.Bits = (irr::u8)pps->getValueDouble("fattycurd.device.bits");
	params.DeviceType = irr::EIDT_BEST;
	params.DisplayAdapter = 0;
	params.Doublebuffer = pps->getValueBool("fattycurd.device.doublebuffer");
	params.DriverMultithreaded = pps->getValueBool("fattycurd.device.multithreaded");
	{
		auto it = c_name_driver.find(pps->getValue("fattycurd.device.drivertype"));
		if (it == c_name_driver.end())
			throw std::runtime_error("illegal driver name");
		params.DriverType = it->second;
	}
	params.Fullscreen = pps->getValueBool("fattycurd.device.fullscreen");
	params.HandleSRGB = pps->getValueBool("fattycurd.device.SRGB");
	params.HighPrecisionFPU = pps->getValueBool("fattycurd.device.highprecisionFPU");
	params.IgnoreInput = false;
	{
		auto it = c_name_log.find(pps->getValue("fattycurd.device.logginglevel"));
		if (it == c_name_log.end())
			throw std::runtime_error("illegal loggging level");
		params.LoggingLevel = it->second;
	}
	params.Stencilbuffer = pps->getValueBool("fattycurd.device.stencilbuffer");
	params.Stereobuffer = pps->getValueBool("fattycurd.device.stereobuffer");
	params.UsePerformanceTimer = pps->getValueBool("fattycurd.device.useperformancetimer");
	params.Vsync = pps->getValueBool("fattycurd.device.vsync");
	{
		irr::u32 width = (irr::u32)pps->getValueDouble("fattycurd.device.windowsize.width");
		irr::u32 height = (irr::u32)pps->getValueDouble("fattycurd.device.windowsize.height");
		params.WindowSize = irr::core::dimension2du(width, height);
	}
	params.WithAlphaChannel = pps->getValueBool("fattycurd.device.withalphachannel");
	params.ZBufferBits = (irr::u8)pps->getValueDouble("fattycurd.device.Zbufferbits");
	params.EventReceiver = m_callback;
	m_device = irr::createDeviceEx(params);
	if (!m_device)
		createFromParamsDevice(m_params);
	else
		m_params = params;
}

void context::loadConfigFromFile(const std::string& name)
{
	if (!m_device)
		createNullDevice();
	auto* xml = m_device->getFileSystem()->createXMLReader(name.c_str());
	if (!xml)
		throw std::runtime_error("CONTEXT cannot open XML file : " + name);
	while (xml->read()) {
		if ((xml->getNodeType() == irr::io::EXN_ELEMENT) &&
			(irr::core::stringw(xml->getNodeName()) ==
			 irr::core::stringw(L"config")))
		{
			while ((xml->getNodeType() != irr::io::EXN_ELEMENT_END) ||
				   (irr::core::stringw(xml->getNodeName()) !=
					irr::core::stringw(L"config")))
			{
				if (!xml->read())
					throw std::runtime_error
					("XML PARSE ERROR : Unexpected end of file in config");
				if (irr::core::stringw(xml->getNodeName()) ==
					irr::core::stringw("config"))
					continue;
				if (irr::core::stringw(xml->getNodeName()) ==
					irr::core::stringw("set-value"))
				{
					xml_set_value* p = xml_set_value::parseSetValue(xml, m_device);
					p->applySetValue();
					delete p;
				} else {
					std::ostringstream oss("");
					std::wstring node_name = xml->getNodeName();
					oss << "XML PARSE ERROR config can only have";
					oss << " set-value item : ";
					oss << std::string(node_name.begin(), node_name.end());
					throw std::runtime_error(oss.str());
				}
			}
		}
	}
	xml->drop();
}

void context::saveConfigToFile(const std::string& name)
{
	auto* xml = m_device->getFileSystem()->createXMLWriter(name.c_str());
	if (!xml)
		throw std::runtime_error("CONTEXT cannot open XML file : " + name);
	xml->writeXMLHeader();
	xml->writeElement(L"config", false);
	xml->writeLineBreak();
	irr::core::array<irr::core::stringw> attrib_name;
	attrib_name.push_back(L"linked-to");
	attrib_name.push_back(L"value");
	{
		irr::core::array<irr::core::stringw> attrib_value;
		attrib_value.push_back(L"fattycurd.device.antialias");
		attrib_value.push_back(irr::core::stringw(m_params.AntiAlias));
		xml->writeElement(L"set-value",
						  true,
						  attrib_name,
						  attrib_value);
		xml->writeLineBreak();
	}
	{
		irr::core::array<irr::core::stringw> attrib_value;
		attrib_value.push_back(L"fattycurd.device.bits");
		attrib_value.push_back(irr::core::stringw(m_params.Bits));
		xml->writeElement(L"set-value",
						  true,
						  attrib_name,
						  attrib_value);
		xml->writeLineBreak();
	}
	{
		irr::core::array<irr::core::stringw> attrib_value;
		attrib_value.push_back(L"fattycurd.device.doublebuffer");
		attrib_value.push_back((m_params.Doublebuffer) ? L"true" : L"false");
		xml->writeElement(L"set-value",
						  true,
						  attrib_name,
						  attrib_value);
		xml->writeLineBreak();
	}
	{
		irr::core::array<irr::core::stringw> attrib_value;
		attrib_value.push_back(L"fattycurd.device.multithreaded");
		attrib_value.push_back((m_params.Doublebuffer) ? L"true" : L"false");
		xml->writeElement(L"set-value",
						  true,
						  attrib_name,
						  attrib_value);
		xml->writeLineBreak();
	}
	{
		irr::core::array<irr::core::stringw> attrib_value;
		attrib_value.push_back(L"fattycurd.device.drivertype");
		auto it = c_driver_name.find
		((const irr::video::E_DRIVER_TYPE)m_params.DriverType);
		attrib_value.push_back(irr::core::stringw(it->second.c_str()));
		xml->writeElement(L"set-value",
						  true,
						  attrib_name,
						  attrib_value);
		xml->writeLineBreak();
	}
	{
		irr::core::array<irr::core::stringw> attrib_value;
		attrib_value.push_back(L"fattycurd.device.fullscreen");
		attrib_value.push_back((m_params.Fullscreen) ? L"true" : L"false");
		xml->writeElement(L"set-value",
						  true,
						  attrib_name,
						  attrib_value);
		xml->writeLineBreak();
	}
	{
		irr::core::array<irr::core::stringw> attrib_value;
		attrib_value.push_back(L"fattycurd.device.SRGB");
		attrib_value.push_back((m_params.HandleSRGB) ? L"true" : L"false");
		xml->writeElement(L"set-value",
						  true,
						  attrib_name,
						  attrib_value);
		xml->writeLineBreak();
	}
	{
		irr::core::array<irr::core::stringw> attrib_value;
		attrib_value.push_back(L"fattycurd.device.highprecisionFPU");
		attrib_value.push_back((m_params.HandleSRGB) ? L"true" : L"false");
		xml->writeElement(L"set-value",
						  true,
						  attrib_name,
						  attrib_value);
		xml->writeLineBreak();
	}
	{
		irr::core::array<irr::core::stringw> attrib_value;
		attrib_value.push_back(L"fattycurd.device.logginglevel");
		auto it = c_log_name.find
		((const irr::ELOG_LEVEL)m_params.LoggingLevel);
		attrib_value.push_back(irr::core::stringw(it->second.c_str()));
		xml->writeElement(L"set-value",
						  true,
						  attrib_name,
						  attrib_value);
		xml->writeLineBreak();
	}
	{
		irr::core::array<irr::core::stringw> attrib_value;
		attrib_value.push_back(L"fattycurd.device.stencilbuffer");
		attrib_value.push_back((m_params.Stencilbuffer) ? L"true" : L"false");
		xml->writeElement(L"set-value",
						  true,
						  attrib_name,
						  attrib_value);
		xml->writeLineBreak();
	}
	{
		irr::core::array<irr::core::stringw> attrib_value;
		attrib_value.push_back(L"fattycurd.device.stereobuffer");
		attrib_value.push_back((m_params.Stereobuffer) ? L"true" : L"false");
		xml->writeElement(L"set-value",
						  true,
						  attrib_name,
						  attrib_value);
		xml->writeLineBreak();
	}
	{
		irr::core::array<irr::core::stringw> attrib_value;
		attrib_value.push_back(L"fattycurd.device.useperformancetimer");
		attrib_value.push_back((m_params.UsePerformanceTimer) ?
							   L"true" :
							   L"false");
		xml->writeElement(L"set-value",
						  true,
						  attrib_name,
						  attrib_value);
		xml->writeLineBreak();
	}
	{
		irr::core::array<irr::core::stringw> attrib_value;
		attrib_value.push_back(L"fattycurd.device.vsync");
		attrib_value.push_back((m_params.Vsync) ? L"true" : L"false");
		xml->writeElement(L"set-value",
						  true,
						  attrib_name,
						  attrib_value);
		xml->writeLineBreak();
	}
	{
		irr::core::array<irr::core::stringw> attrib_value;
		attrib_value.push_back(L"fattycurd.device.windowsize.width");
		attrib_value.push_back(irr::core::stringw(m_params.WindowSize.Width));
		xml->writeElement(L"set-value",
						  true,
						  attrib_name,
						  attrib_value);
		xml->writeLineBreak();
	}
	{
		irr::core::array<irr::core::stringw> attrib_value;
		attrib_value.push_back(L"fattycurd.device.windowsize.height");
		attrib_value.push_back(irr::core::stringw(m_params.WindowSize.Height));
		xml->writeElement(L"set-value",
						  true,
						  attrib_name,
						  attrib_value);
		xml->writeLineBreak();
	}
	{
		irr::core::array<irr::core::stringw> attrib_value;
		attrib_value.push_back(L"fattycurd.device.withalphachannel");
		attrib_value.push_back((m_params.WithAlphaChannel) ?
							   L"true" :
							   L"false");
		xml->writeElement(L"set-value",
						  true,
						  attrib_name,
						  attrib_value);
		xml->writeLineBreak();
	}
	{
		irr::core::array<irr::core::stringw> attrib_value;
		attrib_value.push_back(L"fattycurd.device.Zbufferbits");
		attrib_value.push_back(irr::core::stringw(m_params.ZBufferBits));
		xml->writeElement(L"set-value",
						  true,
						  attrib_name,
						  attrib_value);
		xml->writeLineBreak();
	}
	xml->writeClosingTag(L"config");
	xml->writeLineBreak();
	xml->drop();
}

void context::createFromParamsDevice(const irr::SIrrlichtCreationParameters& params)
{
	if (m_device) m_device->drop();
	m_device = nullptr;
	m_device = irr::createDeviceEx(params);
	if (!m_device)
		throw std::runtime_error("Could not create device!");
	m_params = params;
}

void context::fadeIn(const irr::video::SColor& c, irr::u32 ms)
{
	m_device->setEventReceiver(nullptr);
	irr::video::IVideoDriver* driver = m_device->getVideoDriver();
	irr::gui::IGUIInOutFader* fader = m_device->getGUIEnvironment()->addInOutFader();
	fader->setColor(c);
	fader->fadeIn(ms);
	while(m_device->run() && !fader->isReady()) {
		if (m_device->isWindowActive()) {
			driver->beginScene(true, true, irr::video::SColor(0, 0, 0, 0));
			m_device->getGUIEnvironment()->drawAll();
			driver->endScene();
		}
	}
	fader->remove();
	m_device->setEventReceiver(m_callback);
}

void context::fadeOut(const irr::video::SColor& c, irr::u32 ms)
{
	m_device->setEventReceiver(nullptr);
	irr::video::IVideoDriver* driver = m_device->getVideoDriver();
	irr::gui::IGUIInOutFader* fader = m_device->getGUIEnvironment()->addInOutFader();
	fader->setColor(c);
	fader->fadeOut(ms);
	while(m_device->run() && !fader->isReady()) {
		if (m_device->isWindowActive()) {
			driver->beginScene(true, true, irr::video::SColor(0, 0, 0, 0));
			m_device->getGUIEnvironment()->drawAll();
			driver->endScene();
		}
	}
	fader->remove();
	m_device->setEventReceiver(m_callback);
}
