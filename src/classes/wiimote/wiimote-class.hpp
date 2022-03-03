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

#pragma once

// Libraries
#include <wiiuse/wpad.h>
#include <string>
#include <map>

namespace love {
namespace wiimote {

struct Wiimote {
	int id = 0;

	int status;
	unsigned int extension;
	unsigned int buttonsDown;
	float x, y;

	bool rumbling;
	double rumbleTime;
	double rumbleLength;

	Wiimote();
	void update(int &homePressed);

	std::string getExtension();
	float getX();
	float getY();
	bool isConnected();
	bool isDown(std::string button);
	bool isRumbling();

	bool isClassicDown(std::string button);

	void rumble(bool status);
	void rumbleDuration(int status, double duration);
};

} // wiimote
} // love
