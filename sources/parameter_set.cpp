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

#include "main.h"
#include "parameter_set.h"

parameter_set* parameter_set::m_instance = NULL;

parameter_set::parameter_set() {}

parameter_set::~parameter_set() {}

parameter_set* parameter_set::instance() {
	if (!m_instance)
		m_instance = new parameter_set();
	return m_instance;
}

void parameter_set::release() {
	if (m_instance)
		delete m_instance;
	m_instance = NULL;
}

void parameter_set::setValue(
	const std::string& name,
	const std::string& value)
{
	m_parameter_set[name] = value;
	std::list<parameter_set_listener*>::iterator ite;
	for (ite = m_listener.begin(); ite != m_listener.end(); ++ite)
		(*ite)->operator()(name, m_parameter_set[name]);
}

void parameter_set::setValue(
	const std::string& name,
	const float& value) 
{
	std::stringstream ss("");
	ss << value;
	setValue(name, ss.str());
}

std::string parameter_set::getValue(const std::string& name) {
	if (m_parameter_set.find(name) == m_parameter_set.end())
		return std::string("<unknown>");
	return m_parameter_set[name];
}

double parameter_set::getValueDouble(const std::string& name) {
	std::stringstream ss("");
	std::string strValue = getValue(name);
	ss << strValue;
	double ret = 0.0;
	ss >> ret;
	return ret;
}

void parameter_set::addListener(parameter_set_listener* psl) {
	m_listener.push_back(psl);
}

void parameter_set::removeListener(parameter_set_listener* psl) {
	m_listener.remove(psl);
}

void parameter_set::dump(std::ostream& os) {
	std::map<std::string, std::string>::iterator ite;
	for (ite = m_parameter_set.begin(); ite != m_parameter_set.end(); ++ite)
		os << "[" << ite->first << "] := <" << ite->second << ">" << std::endl;
}