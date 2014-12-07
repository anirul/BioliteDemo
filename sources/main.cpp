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

#include <thread>

#include "main.h"
#include "win.h"
#include "context.h"
#include "parameter_set.h"

#ifdef _IRR_OSX_PLATFORM_
	#include <CoreFoundation/CFUserNotification.h>
	#include <unistd.h>
#endif

#ifdef _IRR_WINDOWS_
	#pragma comment(lib, "Irrlicht.lib")
	#pragma comment(lib, "IrrKlang.lib")
	#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

int main(int ac, char** av) {
#ifdef _IRR_WINDOWS_
	HWND hwnd = NULL;
#endif
	context* ctx = nullptr;
	irr_win* win = nullptr;
	try {
		win = new irr_win(XML_MENU_FILE);
		ctx = win->getContext();
		while (win->runOnce(ctx->m_device))
			std::this_thread::yield();
	} catch (const std::runtime_error& ex) {
		std::cerr << ex.what() << std::endl;
		// TODO maybe suppress all this as I am displaying a message above
		std::ostringstream oss("");
		oss << ex.what() << std::endl;
#ifdef _IRR_WINDOWS_
		MessageBoxA(hwnd, oss.str().c_str(), "Exception", MB_ICONEXCLAMATION);
#endif
#ifdef _IRR_OSX_PLATFORM_
		if (ctx && ctx->m_device && ctx->m_device->isFullscreen()) {
			// to avoid popup window hidden behind the game
			ctx->m_device->closeDevice();
		}
		CFOptionFlags responseFlags = 0;
		CFStringRef errorMsg = CFStringCreateWithCString(kCFAllocatorDefault,
														 oss.str().c_str(),
														 kCFStringEncodingASCII);
		CFUserNotificationDisplayAlert(20.0,
									   3,
									   NULL,
									   NULL,
									   NULL,
									   CFSTR("Error"),
									   errorMsg,
									   CFSTR("OK"),
									   NULL,
									   NULL,
									   &responseFlags);
#endif
	}
	if (win) delete win;
	return 0;
}

