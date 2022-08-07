/* WiiLÖVE wiimote module
 *
 * This file is part of WiiLÖVE.
 *
 * Copyright (C) 2022  HTV04
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program.  If not, see
 * <https://www.gnu.org/licenses/>.
 */

#ifndef HW_DOL

// Libraries
#include <sol/sol.hpp>
#include <ogc/conf.h>
#include <wiiuse/wpad.h>
#include <vector>
#include <map>
#include <utility>

// Header
#include "wiimote.hpp"

namespace love {
namespace wiimote {

WPADData *data;

struct Wiimote {
	int status = WPAD_ERR_NONE;
	unsigned int extension;
	unsigned int buttonsDown;
	float x, y;
	float angle;

	bool rumbling;
};

std::vector<Wiimote> wiimotes(4);

std::map<std::string, int> buttonMap = {
	{"2", WPAD_BUTTON_2},
	{"1", WPAD_BUTTON_1},
	{"b", WPAD_BUTTON_B},
	{"a", WPAD_BUTTON_A},
	{"-", WPAD_BUTTON_MINUS},
	{"left", WPAD_BUTTON_LEFT},
	{"right", WPAD_BUTTON_RIGHT},
	{"up", WPAD_BUTTON_UP},
	{"down", WPAD_BUTTON_DOWN},
	{"+", WPAD_BUTTON_PLUS},
	{"z", WPAD_NUNCHUK_BUTTON_Z},
	{"c", WPAD_NUNCHUK_BUTTON_C}
};

std::map<std::string, int> classicButtonMap = {
	{"up", WPAD_CLASSIC_BUTTON_UP},
	{"left", WPAD_CLASSIC_BUTTON_LEFT},
	{"zr", WPAD_CLASSIC_BUTTON_ZR},
	{"x", WPAD_CLASSIC_BUTTON_X},
	{"a", WPAD_CLASSIC_BUTTON_A},
	{"y", WPAD_CLASSIC_BUTTON_Y},
	{"b", WPAD_CLASSIC_BUTTON_B},
	{"zl", WPAD_CLASSIC_BUTTON_ZL},
	{"r", WPAD_CLASSIC_BUTTON_FULL_R},
	{"+", WPAD_CLASSIC_BUTTON_PLUS},
	{"-", WPAD_CLASSIC_BUTTON_MINUS},
	{"l", WPAD_CLASSIC_BUTTON_FULL_L},
	{"down", WPAD_CLASSIC_BUTTON_DOWN},
	{"right", WPAD_CLASSIC_BUTTON_RIGHT}
};

std::map<int, std::string> extensionMap = {
	{WPAD_EXP_NONE, "None"},
	{WPAD_EXP_NUNCHUK, "Nunchuk"},
	{WPAD_EXP_CLASSIC, "Classic Controller"}
};

// Internal functions
void init() {
	WPAD_Init();
	WPAD_SetVRes(0, 640, 480);
	WPAD_SetDataFormat(WPAD_CHAN_ALL, WPAD_FMT_BTNS_ACC_IR);

	WPAD_ScanPads();
}
void update(std::vector<bool> &adds, std::vector<bool> &removes, int &homePressed) {
	unsigned int buttonsPressed;

	WPAD_ScanPads();

	for (int i = 0; i <= 3; i++) {
		int status = WPAD_Probe(i, NULL);

		if (wiimotes[i].status == WPAD_ERR_NONE && status != WPAD_ERR_NONE) {
			removes[i] = true;
		} else if (wiimotes[i].status != WPAD_ERR_NONE && status == WPAD_ERR_NONE) {
			adds[i] = true;
		}
		wiimotes[i].status = status;

		if (status == WPAD_ERR_NONE) {
			data = WPAD_Data(i);
			buttonsPressed = data->btns_d;

			wiimotes[i].buttonsDown = data->btns_h;
			wiimotes[i].x = data->ir.x;
			wiimotes[i].y = data->ir.y;
			wiimotes[i].angle = data->ir.angle;
			wiimotes[i].extension = data->exp.type;

			if ((buttonsPressed & WPAD_BUTTON_HOME) || (buttonsPressed & WPAD_CLASSIC_BUTTON_HOME)) { homePressed = i; }
		}
	}
}

namespace module {

// Wii Remote querying functions
float getAngle(int id) { return wiimotes[id].angle; }
std::string getExtension(int id) {
	return extensionMap[wiimotes[id].extension];
}
std::pair<float, float> getPosition(int id) {
	return std::make_pair(wiimotes[id].x, wiimotes[id].y);
}
float getX(int id) { return wiimotes[id].x; }
float getY(int id) { return wiimotes[id].y; }
bool isConnected(int id) {
	return wiimotes[id].status == WPAD_ERR_NONE;
}
bool isDown(int id, std::string button) {
	return wiimotes[id].buttonsDown & buttonMap[button];
}
bool isRumbling(int id) { return wiimotes[id].rumbling; }

// Classic Controller querying functions
bool isClassicDown(int id, std::string button) {
	return wiimotes[id].buttonsDown & classicButtonMap[button];
}

// Actions
bool setRumble(int id, bool status) {
	if (WPAD_Rumble(id, status) == WPAD_ERR_NONE) {
		wiimotes[id].rumbling = status;

		return true;
	} else {
		return false;
	}
}

} // module
} // wiimote
} // love

#endif // !HW_DOL
