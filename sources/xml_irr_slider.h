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

#ifndef BIOLITE_XML_IRR_SLIDER_HEADER_DEFINED
#define BIOLITE_XML_IRR_SLIDER_HEADER_DEFINED

#include "main.h"
#include "xml_misc.h"
#include "xml_element.h"

class xml_irr_slider : public xml_element {
private :
	irr::gui::IGUIScrollBar* m_scrollbar;
	irr::core::vector2df m_position;
	irr::core::vector2df m_size;
	xml_misc::coordinate_type m_ct;
	irr::s32 m_scroll_pos;
	std::string m_linked_to;
public :
	static xml_irr_slider* parseSlider(
		irr::io::IXMLReaderUTF8* xml, 
		irr::IrrlichtDevice* pdevice);
public :
	xml_irr_slider(
		irr::IrrlichtDevice* pdevice,
		const std::string& linked_to,
		const irr::core::vector2df& position,
		const irr::core::vector2df& size,
		irr::s32 scroll_pos,
		xml_misc::coordinate_type ct = xml_misc::absolute);
	virtual ~xml_irr_slider();
public :
	//! from xml_element
	virtual bool isIn(const irr::core::vector2d<irr::s32>& mouse) const;
	virtual irr::s32 addElement(
		irr::gui::IGUIEnvironment* penv,
		irr::gui::IGUIElement* parent = NULL);
	virtual xml_misc::element_type getType() const 
		{ return xml_misc::XML_IRR_SLIDER; }
	virtual bool OnEvent(const irr::SEvent& event);
};

#endif // BIOLITE_XML_IRR_SLIDER_HEADER_DEFINED