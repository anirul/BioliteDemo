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

#ifndef BIOLITE_PARAMETER_SET_HEADER_DEFINED
#define BIOLITE_PARAMETER_SET_HEADER_DEFINED

#include "main.h"

//! listen for a value
struct parameter_set_listener {
	virtual void operator()(
		const std::string& name,
		const std::string& value) = 0;
};

//! set of parameter with a callback listener is suppose to be replaced by
//! a DHT for network enabling
class parameter_set {
private :
	std::map<std::string, std::string> m_parameter_set;
	std::list<parameter_set_listener*> m_listener;
	static parameter_set* m_instance;
protected :
	parameter_set();
public :
	virtual ~parameter_set();
	static parameter_set* instance();
	static void release();
	void setValue(
		const std::string& name, 
		const std::string& value);
	void setValue(
		const std::string& name,
		const float& value);
	std::string getValue(const std::string& name);
	double getValueDouble(const std::string& name);
	void addListener(parameter_set_listener* psl);
	void removeListener(parameter_set_listener* psl);
	void dump(std::ostream& os);
};

#endif // BIOLITE_PARAMETER_SET_HEADER_DEFINED