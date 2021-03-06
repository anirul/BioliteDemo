/*
 * Copyright (c) 2010, mic
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

#include "media_collection.h"

#include <irrlicht.h>

#ifdef _IRR_OSX_PLATFORM_
#include <CoreFoundation/CoreFoundation.h>
#include <CoreFoundation/CFBundle.h>
#endif


std::string getPathOfMedia(const char* filename, const char* fileExtension) {
#ifdef _IRR_OSX_PLATFORM_
	char path[1024];
	CFURLRef cfFileURL = CFBundleCopyResourceURL (
		CFBundleGetMainBundle(),
		CFStringCreateWithCString(
			kCFAllocatorDefault, 
			filename, 
			kCFStringEncodingASCII),
		0,
		CFSTR("media"));
	CFURLGetFileSystemRepresentation(
		cfFileURL,
		true,
		(UInt8*)path,
		1023);
									 
	return std::string (path);
#else
	std::string path("./media/");
	path.append(filename);
	return path;
	//snprintf(path, 1024*sizeof(char), "./media/%s", filename);
#endif
}