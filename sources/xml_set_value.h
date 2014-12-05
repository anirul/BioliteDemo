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

#ifndef XML_BIOLITE_SET_VALUE_HEADER_DEFINED
#define XML_BIOLITE_SET_VALUE_HEADER_DEFINED

#include "main.h"
#include "xml_element.h"
#include "xml_misc.h"
#include "parameter_set.h"

class xml_set_value : public xml_element {
private :
	std::string m_linked_to;
	std::string m_value;
public :
	static xml_set_value* parseSetValue(
		irr::io::IXMLReaderUTF8* xml, 
		irr::IrrlichtDevice* pdevice);
public :
	xml_set_value(
		irr::IrrlichtDevice* pdevice,
		const std::string& tag,
		const std::string& value);
	virtual ~xml_set_value();
public :
	void applySetValue();
public :
	//! from xml_element
	virtual bool isIn(const irr::core::vector2d<irr::s32>& mouse) const
		{ return false; }
	virtual irr::s32 addElement(
		irr::gui::IGUIEnvironment* penv, 
		irr::gui::IGUIElement* parent = NULL);
	virtual xml_misc::element_type getType() const 
		{ return xml_misc::XML_SET_VALUE; }
	virtual bool OnEvent(const irr::SEvent& event) 
		{ return false; }	
};

#endif // XML_BIOLITE_SET_VALUE_HEADER_DEFINED