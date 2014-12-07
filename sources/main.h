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

#ifndef BIOLITE_MAIN_HEADER_DEFINED
#define BIOLITE_MAIN_HEADER_DEFINED

#define VERSION_LENGTH 9
#define EPSILON 1e-4f
#define WIDTH 1024
#define HEIGHT 768
#define SOCK_PORT 4242
#define MAX_CLIENT 4
#define MAX_PLAYER_NAME 32
#define MAX_HOST_NAME 32
#define WM_NET (WM_USER + 100)
#define TIMER_UPDATE 100
#define ANGULAR_SUN_ROTATION 0.01f
#define ROT_FACTOR 0.03f
#define EMPTY_TEXT "<empty>"
#define AI_TEST_PER_TICK 4  // 10 very evil 1: very WEAK noob lame
#ifndef _DEBUG
	#define FULLSCREEN
#endif

// planet generation parameters
#define NBSUB 5
#define GENSTEP 32
#define NBGEN 5120

// arrow colors
#define COLOR_ENNEMI	irr::video::SColor(0x3fff0000);
#define COLOR_SELF		irr::video::SColor(0x3f00ff00);
#define COLOR_EMPTY		irr::video::SColor(0x3f0000ff);

// game rules
#define BORDER_RANGE 0.15f
#define ACTION_RANGE 0.4f
#define BOOST_ON_CLICK 100
#define DESTROY_ON_CLICK 100
#define CLICK_COST 40
#define GROW_ON_TICK 5
#define SEED_PROD 0.5f
// #define WITH_MUSIC
#define INITIAL_SEED 4
#define INITIAL_CLICK CLICK_COST*INITIAL_SEED
#define INITIAL_SIZE 255
#define XML_MENU_FILE "biolite.xml"

#ifndef WIN32
	#define FD_ACCEPT 1
	#define FD_CONNECT 2
	#define FD_READ 3
	#define FD_CLOSE 4
	#define SOCKET_ERROR -1
#endif

#include <assert.h>
#include <irrlicht.h>
#include <irrXML.h>

#ifndef WIN32
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netdb.h>
#else
	#include <time.h>
	#include <WinSock.h>
	#include <Windows.h>
#endif

#include <exception>
#include <string>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <list>
#include <map>
#include <stack>
#include <vector>
#include <string>
#include <fstream>

#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif

#include "ISceneNodeAnimatorCameraArcball.h"

#ifdef WIN32
    #define PERSISTANT_SET_SUBKEY "SoftWare\\Uncultured Games\\Biolite\\BioliteDemo"
#endif

#endif // BIOLITE_MAIN_HEADER_DEFINED
