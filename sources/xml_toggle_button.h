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

#ifndef BIOLITE_XML_TOGGLE_BUTTON_HEADER_DEFINED
#define BIOLITE_XML_TOGGLE_BUTTON_HEADER_DEFINED

#include "main.h"
#include "xml_misc.h"
#include "xml_element.h"
#include "parameter_set.h"

class xml_toggle_button : public xml_element, public parameter_set_listener {
private :
	irr::video::ITexture* m_texture_up;
	irr::video::ITexture* m_texture_down;
	irr::video::ITexture* m_texture_up_flyby;
	irr::video::ITexture* m_texture_down_flyby;
	irr::core::vector2df m_position;
	irr::core::vector2df m_size;
	xml_misc::coordinate_type m_ct;
	xml_misc::toggle_button_state_type m_bst;
	std::string m_linked_to;
	std::string m_value;
	irr::IrrlichtDevice* m_pdevice;
public :
	//! create xml_toggle_button from xml
	static xml_toggle_button* parseToggleButton(
		irr::io::IXMLReaderUTF8* xml,
		irr::IrrlichtDevice* pdevice);
public :
	xml_toggle_button(
		irr::IrrlichtDevice* pdevice,
		const std::string& linked_to,
		const std::string& value,
		const irr::core::vector2df& position,
		const irr::core::vector2df& size,
		const std::string& path_up,
		const std::string& path_down,
		const std::string& path_up_flyby,
		const std::string& path_down_flyby,
		xml_misc::coordinate_type ct = xml_misc::absolute);
	virtual ~xml_toggle_button();
public :
	//! from parameter_set_listener
	virtual void operator()(const std::string& name, const std::string& value);
	//! from xml_element
	virtual bool isIn(const irr::core::vector2d<irr::s32>& mouse) const;
	virtual irr::s32 addElement(
		irr::gui::IGUIEnvironment* penv,
		irr::gui::IGUIElement* parent = NULL);
	virtual xml_misc::element_type getType() const 
		{ return xml_misc::XML_TOGGLE_BUTTON; }
	virtual bool OnEvent(const irr::SEvent& event);
protected :
	xml_misc::toggle_button_state_type getButtonState() const { return m_bst; }
	void setButtonState(xml_misc::toggle_button_state_type bst) { m_bst = bst; }
	irr::s32 addImageUp(
		irr::gui::IGUIEnvironment* penv,
		irr::gui::IGUIElement* parent = NULL);
	irr::s32 addImageDown(
		irr::gui::IGUIEnvironment* penv,
		irr::gui::IGUIElement* parent = NULL);
	irr::s32 addImageUpFlyby(
		irr::gui::IGUIEnvironment* penv,
		irr::gui::IGUIElement* parent = NULL);
	irr::s32 addImageDownFlyby(
		irr::gui::IGUIEnvironment* penv,
		irr::gui::IGUIElement* parent = NULL);
};

#endif // BIOLITE_XML_TOGGLE_BUTTON_HEADER_DEFINED
