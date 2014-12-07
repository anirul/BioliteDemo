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
#include "persistant_set.h"
#ifdef _IRR_WINDOWS_
#include <tchar.h>
#endif // _IRR_WINDOWS_

persistant_set* persistant_set::m_instance = NULL;

persistant_set::persistant_set() {}

persistant_set::~persistant_set() {}

persistant_set* persistant_set::instance() {
	if (!m_instance)
		m_instance = new persistant_set();
	return m_instance;
}

void persistant_set::release() {
	if (m_instance)
		delete m_instance;
	m_instance = NULL;
}

void persistant_set::setValue(
	const std::string& name,
	const std::string& value)
{
#ifdef _IRR_WINDOWS_
	HKEY hkey;
	LPDWORD dwStatus = 0;
	DWORD dw = 0;
	int longueur = 0;

	if ( ERROR_SUCCESS == RegCreateKeyEx(HKEY_CURRENT_USER, PERSISTANT_SET_SUBKEY, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, dwStatus))
	{
		longueur = value.length();
		dw = (DWORD)((longueur+1)*sizeof(TCHAR));
        RegSetValueEx( hkey, name.c_str(), NULL, REG_SZ, (BYTE *) value.c_str(), dw );
	}
    RegCloseKey(hkey);
#endif
	std::list<persistant_set_listener*>::iterator ite;
	for (ite = m_listener.begin(); ite != m_listener.end(); ++ite)
		(*ite)->operator()(name, value);
}

std::wstring persistant_set::getValue(const std::string& name) {
	std::wstring value = L"";
#ifdef _IRR_WINDOWS_
	HKEY hkey;
	DWORD dw = 0;
	int status = 0;
	int stringSize = 0;
	std::wstring space = L" ";

	if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_CURRENT_USER, PERSISTANT_SET_SUBKEY, 0, KEY_QUERY_VALUE, &hkey) )
	{
	    // Retrieve the value
		// The call is made to times because the length of the string is unknown
		// The first will fail and then the length will be stored in dw
        status = RegQueryValueEx(hkey, name.c_str(), NULL, NULL, (LPBYTE)value.c_str(), &dw);
		while (status == ERROR_MORE_DATA)
		{
		    // we have to query with the good string size otherwise it won't work properly
			stringSize = dw/2-1;
			while (stringSize--)
		    {
			    value = value + space;
			}
            status = RegQueryValueEx(hkey, name.c_str(), NULL, NULL, (LPBYTE)value.c_str(), &dw);
        }
        if (status != 0)
            value = L"<unknown>";

	    RegCloseKey(hkey);
	}
#endif
	return std::wstring(value);
}

double persistant_set::getValueDouble(const std::string& name) {
	double value = 0.0;
#ifdef _IRR_WINDOWS_

	HKEY hkey;
	DWORD dw = 0;
	int status = 0;
	if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_CURRENT_USER, PERSISTANT_SET_SUBKEY, 0, KEY_QUERY_VALUE, &hkey) )
	{
	    // Retrieve the value
        status = RegQueryValueEx(hkey, name.c_str(), NULL, NULL, (LPBYTE)&value, &dw);
	    RegCloseKey(hkey);
	}
#endif
	return value;
}

void persistant_set::addListener(persistant_set_listener* psl) {
	m_listener.push_back(psl);
}

void persistant_set::removeListener(persistant_set_listener* psl) {
	m_listener.remove(psl);
}

void persistant_set::dump(std::ostream& os) {
#ifdef _IRR_WINDOWS_
	//TODO
#endif
}