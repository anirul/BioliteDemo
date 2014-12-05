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
#include "win.h"
#include "resolution_chooser.h"
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
	irr::IrrlichtDevice* pdevice = NULL;
	irr::SIrrlichtCreationParameters params;
	try {
		{ // resolution chooser
			pdevice = irr::createDevice(
				irr::video::EDT_OPENGL,
				irr::core::dimension2d<unsigned int>(320, 200),
				16,
				false,
				false,
				true);
			if (!pdevice) 
				throw std::runtime_error(
					"ERROR : Could not create device (resolution chooser)");
#ifdef _IRR_WINDOWS_
			if (!pdevice->getFileSystem()->addZipFileArchive("media.zip"))
				throw std::runtime_error("ERROR : cannot open [media.zip]");
#endif
			pdevice->setResizable(false);
			irr_resolution_chooser irc(pdevice);
			while (irc.runOnce(pdevice))
#ifndef _IRR_WINDOWS_
				usleep(1);
#else
				Sleep(1);
#endif
			params = irc.getParameters();
			pdevice->drop();
			pdevice = NULL;
			if (!irc.ok()) return 0;
		}
		{ // start the actual window
			pdevice =  irr::createDevice(
				params.DriverType,
				params.WindowSize,
				params.Bits,
				params.Fullscreen,
				params.Stencilbuffer,
				params.Vsync);
			if (!pdevice) 
				throw std::runtime_error(
					"ERROR : Could not create device (main)");
#ifdef _IRR_WINDOWS_
			if (!pdevice->getFileSystem()->addZipFileArchive("media.zip", true, true))
				throw std::runtime_error("ERROR : cannot open [media.zip]");
#endif
			pdevice->setResizable(false);
#ifdef _IRR_WINDOWS_
			irr::video::SExposedVideoData evd = 
				pdevice->getVideoDriver()->getExposedVideoData();
			switch (pdevice->getVideoDriver()->getDriverType()) {
			case irr::video::EDT_DIRECT3D9 :
				hwnd = (HWND)evd.D3D9.HWnd;
				break;
			case irr::video::EDT_DIRECT3D8 :
				hwnd = (HWND)evd.D3D8.HWnd;
				break;
			case irr::video::EDT_OPENGL :
				hwnd = (HWND)evd.OpenGLWin32.HWnd;
				break;
			default :
				break;
			}
#endif
			pdevice->setWindowCaption(L"Biolite - Irrlicht Version");
			irr_win main_logic(pdevice);
			while (main_logic.runOnce(pdevice)) 
#ifndef _IRR_WINDOWS_
				usleep(1);
#else
				Sleep(1);
#endif
		} // end of the main window
	} catch (const std::runtime_error& ex) {
		parameter_set::instance()->dump(std::cerr);
		std::cerr << ex.what() << std::endl;
		// TODO maybe suppress all this as I am displaying a message above
#ifdef _IRR_WINDOWS_
		std::ostringstream oss("");
		oss << ex.what() << std::endl;
		parameter_set::instance()->dump(std::cerr);
		MessageBoxA(hwnd, oss.str().c_str(), "Exception", MB_ICONEXCLAMATION);
#endif
#ifdef _IRR_OSX_PLATFORM_
		if (pdevice && pdevice->isFullscreen()) {
			// to avoid popup window hidden behind the game
			pdevice->closeDevice(); 
		}
		std::ostringstream oss("");
		oss << ex.what() << std::endl;
		parameter_set::instance()->dump(std::cerr);
		CFOptionFlags responseFlags = 0;
		CFStringRef errorMsg = CFStringCreateWithCString(
			kCFAllocatorDefault, 
			oss.str().c_str(), 
			kCFStringEncodingASCII);
		CFUserNotificationDisplayAlert(
			20.0, 
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
	return 0;
}

