/* WiiLÖVE Wiimote class
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

// Libraries
#include <wiiuse/wpad.h>
#include <map>
#include <string>
#include <utility>
#include <stdexcept>

// Header
#include "wiimote-class.hpp"

namespace {
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
}

namespace love {
namespace wiimote {

// Constructor
Wiimote::Wiimote() {}

// Internal functions
void Wiimote::update(int curStatus, int &homePressed) {
	WPADData *data;
	unsigned int buttonsPressed;

	status = curStatus;

	if (status == WPAD_ERR_NONE) {
		data = WPAD_Data(id);
		buttonsPressed = data->btns_d;

		buttonsDown = data->btns_h;
		x = data->ir.x;
		y = data->ir.y;
		angle = data->ir.angle;

		if (buttonsPressed & WPAD_BUTTON_HOME || buttonsPressed & WPAD_CLASSIC_BUTTON_HOME) { homePressed = id; }
	}
}

// Wii Remote querying functions
float Wiimote::getAngle() { return angle; }
std::string Wiimote::getExtension() {
	return extensionMap[extension];
}
int Wiimote::getID() { return id; }
std::pair<float, float> Wiimote::getPosition() {
	return std::make_pair(x, y);
}
float Wiimote::getX() { return x; }
float Wiimote::getY() { return y; }
bool Wiimote::isConnected() {
	return status == WPAD_ERR_NONE;
}
bool Wiimote::isDown(std::string button) {
	return buttonsDown & buttonMap[button];
}
bool Wiimote::isRumbling() { return rumbling; }

// Classic Controller querying functions
bool Wiimote::isClassicDown(std::string button) {
	return buttonsDown & classicButtonMap[button];
}

// Actions
bool Wiimote::setRumble(bool status) {
	if (WPAD_Rumble(id, status) == WPAD_ERR_NONE) {
		rumbling = status;

		return true;
	} else {
		rumbling = false;

		return false;
	}
}

} // wiimote
} // love
