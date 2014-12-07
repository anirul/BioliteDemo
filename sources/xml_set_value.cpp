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

#include "xml_set_value.h"

xml_set_value* xml_set_value::parseSetValue(irr::io::IXMLReader* xml,
											irr::IrrlichtDevice* pdevice)
{
	parameter_set* pps = parameter_set::instance();
	std::wstring linked_to(L"");
	std::wstring value(L"");
	if (xml->getAttributeCount() != 2) {
		std::ostringstream oss("");
		oss << "XML PARSE ERROR in file (";
		oss << pps->getValue("fattycurd.xml.file");
		oss << ") : Number of xml attibutes of set value must ";
		oss << " be 2 is : ";
		oss << xml->getAttributeCount();
		oss << std::endl;
		for (unsigned int i = 0; i < xml->getAttributeCount(); ++i) {
			oss << "   [" << xml->getAttributeName(i);
			oss << ":" << xml->getAttributeValue(i);
			oss << "]" << std::endl;
		}
		throw std::runtime_error(oss.str());
	}
	for (unsigned int i = 0; i < xml->getAttributeCount(); ++i) {
		irr::core::stringw attrib = xml->getAttributeName(i);
		if (attrib == irr::core::stringw(L"linked-to")) {
			linked_to = xml->getAttributeValue(i);
			continue;
		}
		if (attrib == irr::core::stringw(L"value")) {
			value = xml->getAttributeValue(i);
			continue;
		}
		std::ostringstream oss("");
		oss << "XML PARSE ERROR in file (";
		oss << pps->getValue("fattycurd.xml.file");
		oss << ") : Unknown attributes in set value : ";
		oss << xml->getAttributeName(i);
		throw std::runtime_error(oss.str().c_str());
	}
	if (!linked_to.size()) {
		std::ostringstream oss("");
		oss << "XML PARSE ERROR in file (";
		oss << pps->getValue("fattycurd.xml.file");
		oss << ") : missing [linked-to] in set value";
		throw std::runtime_error(oss.str().c_str());
	}
	if (!value.size()) {
		std::ostringstream oss("");
		oss << "XML PARSE ERROR in file (";
		oss << pps->getValue("fattycurd.xml.file");
		oss << ") : missing [value] in set value";
		throw std::runtime_error(oss.str().c_str());
	}
	xml_set_value* set_value = new xml_set_value(pdevice,
												 std::string(linked_to.begin(), linked_to.end()),
												 std::string(value.begin(), value.end()));
	return set_value;
}


xml_set_value* xml_set_value::parseSetValue(
	irr::io::IXMLReaderUTF8* xml,
	irr::IrrlichtDevice* pdevice)
{
	std::string linked_to("");
	std::string value("");
	if (xml->getAttributeCount() != 2) {
		std::stringstream ss("");
		ss << "XML PARSE ERROR : Number of xml attibutes of set value must ";
		ss << " be 2 is : ";
		ss << xml->getAttributeCount();
		ss << std::endl;
		for (unsigned int i = 0; i < xml->getAttributeCount(); ++i) {
			ss << "   [" << xml->getAttributeName(i);
			ss << ":" << xml->getAttributeValue(i) << std::endl;
		}
		throw std::runtime_error(ss.str());
	}
	for (unsigned int i = 0; i < xml->getAttributeCount(); ++i) {
		std::string attrib = xml->getAttributeName(i);
		if (attrib == std::string("linked-to")) {
			linked_to = xml->getAttributeValue(i);
			continue;
		}
		if (attrib == std::string("value")) {
			value = xml->getAttributeValue(i);
			continue;
		}
		std::ostringstream oss("");
		oss << "XML PARSER ERROR : Unknown attributes in set value : ";
		oss << xml->getAttributeName(i);
		throw std::runtime_error(oss.str().c_str());
	}
	if (!linked_to.size())
		throw std::runtime_error(
			"XML PARSER ERROR : missing [linked-to] in set value");
	if (!value.size())
		throw std::runtime_error(
			"XML PARSER ERROR : missing [value] in set value");
	xml_set_value* set_value = new xml_set_value(
		pdevice,
		linked_to,
		value);
	return set_value;
}

xml_set_value::~xml_set_value() {}

xml_set_value::xml_set_value(
	irr::IrrlichtDevice* pdevice,
	const std::string& linked_to,
	const std::string& value)
	:	m_linked_to(linked_to),
		m_value(value) {}

void xml_set_value::applySetValue() {
	parameter_set::instance()->setValue(m_linked_to, m_value);
}

irr::s32 xml_set_value::addElement(
		irr::gui::IGUIEnvironment* penv, 
		irr::gui::IGUIElement* parent) 
{
	applySetValue();
	return -1;
}
