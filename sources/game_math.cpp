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
#include "game_math.h"

irr::video::SColor string2color(const std::string& name) {
	irr::video::SColor color;
	color.setAlpha(0xff);
	if (name == std::string("black")) {
		color.setRed(0x00);
		color.setGreen(0x00);
		color.setBlue(0x00);
	}
	if (name == std::string("orange")) {
		color.setRed(0xff);
		color.setGreen(0x7f);
		color.setBlue(0x00);
	}
	if (name == std::string("red")) {
		color.setRed(0xff);
		color.setGreen(0x00);
		color.setBlue(0x00);
	}
	if (name == std::string("green")) {
		color.setRed(0x00);
		color.setGreen(0xff);
		color.setBlue(0x00);
	}
	if (name == std::string("blue")) {
		color.setRed(0x00);
		color.setGreen(0x00);
		color.setBlue(0xff);
	}
	if (name == std::string("yellow")) {
		color.setRed(0xff);
		color.setGreen(0xff);
		color.setBlue(0x00);
	}
	if (name == std::string("gray")) {
		color.setRed(0x7f);
		color.setGreen(0x7f);
		color.setBlue(0x7f);
	}
	if (name == std::string("white")) {
		color.setRed(0xff);
		color.setGreen(0xff);
		color.setBlue(0xff);
	}
	if (name == std::string("pink")) {
		color.setRed(0xe6);
		color.setGreen(0x57);
		color.setBlue(0xbe);
	}
	if (name == std::string("purple")) {
		color.setRed(0xa4);
		color.setGreen(0x65);
		color.setBlue(0xf3);
	}
	if (name == std::string("cyan")) {
		color.setRed(0x56);
		color.setGreen(0xcd);
		color.setBlue(0xc0);
	}
	return color;
}

std::string color2string(const irr::video::SColor& color) {
	return std::string("white");
}

long alea::stdmin() {
	long haut, bas, inter;
	bas = 16807 * (m_alea_var & 0xffffL);
	haut = 16807 * (m_alea_var >> 16);
	inter = (bas >> 16) + haut;
	bas = ((bas & 0xffff) | ((inter & 0x7fff)<<16)) +
	(inter >> 15);
	if ((bas & 0x80000000L) != 0)
		bas = (bas + 1) & 0x7fffffffL;
	m_alea_var = bas;
	return bas;
}

unsigned long hash(const std::string& str, unsigned long mask) {
	unsigned long h = 0;
	for (unsigned long i = 0; i < str.size(); ++i) {
		unsigned long highorder = h & 0xf8000000;
		h = h << 5;
		h = h ^ (highorder >> 27);
		h = h ^ str.at(i);
	}
	return h % mask;
}

void get_color(irr::video::SColor& col, int i) {
	if (i < 0 || i >= MAX_CLIENT) {
		col = irr::video::SColor(255, 85, 85, 170);
	} else {
		col = irr::video::SColor(255, 128, 128, 128);
		if (i & 1)
			col.setRed(255);
		if (i & 2)
			col.setGreen(255);
		if (i & 4)
			col.setBlue(255);
	}
}

