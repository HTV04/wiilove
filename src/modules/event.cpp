/* WiiLÖVE event module
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
#include "../lib/sol.hpp"
#include <vector>
#include <tuple>
#include <grrlib.h>
#include <cstdlib>

// Classes
#include "../classes/wiimote/wiimote-class.hpp"

// Modules
#include "wiimote.hpp"

// Header
#include "event.hpp"

namespace {
	std::vector<std::tuple<sol::object, sol::object, sol::object, sol::object, sol::object, sol::object, sol::object>> events;

	void preQuit() { // Perform pre-quit tasks
		// Be a good boy, clear the memory allocated by GRRLIB
		GRRLIB_Exit();
	}

	int resetFunc = -1;
	void resetPressed(unsigned int irq, void *ctx) { resetFunc = SYS_RETURNTOMENU; }
	void powerPressed() { resetFunc = SYS_POWEROFF; }
	void wiimotePowerPressed(int id) { resetFunc = SYS_POWEROFF; }
}

namespace love {
namespace event {

void init () {
	// SYS_ResetSystem callbacks
	SYS_SetResetCallback(resetPressed);
	SYS_SetPowerCallback(powerPressed);
	WPAD_SetPowerButtonCallback(wiimotePowerPressed);
}

namespace module {

// Event functions, aka WiiLÖVE's "master update"
// NOTE: We have to add events in reverse!
void pump(sol::this_state s) {
	// Check if resetFunc has a value
	if (resetFunc > -1) { // Run reset function
		preQuit();

		// NOTE: Games won't be able to detect if a hardware button is pressed and prevent its
		//       function. This is by design because it functions as a failsafe in case there is
		//       no other way to exit a game. Developers shouldn't rely on this because unsaved
		//       data will be lost!
		SYS_ResetSystem(resetFunc, 0, 0);
	}

	std::vector<bool> wiimoteAdds {false, false, false, false};
	std::vector<bool> wiimoteRemoves {false, false, false, false};

	int homePressed = -1;

	events.push_back(std::make_tuple(sol::nil, sol::nil, sol::nil, sol::nil, sol::nil, sol::nil, sol::nil));

	// Update wiimotes
	love::wiimote::update(wiimoteAdds, wiimoteRemoves, homePressed);

	if (homePressed != -1) {
		events.push_back(std::make_tuple(sol::make_object(s, "homepressed"), sol::make_object(s, homePressed), sol::nil, sol::nil, sol::nil, sol::nil, sol::nil));
	}
	for (int i = 3; i >= 0; i--) {
		if (wiimoteAdds[i]) {
			events.push_back(std::make_tuple(sol::make_object(s, "wiimoteconnected"), sol::make_object(s, i), sol::nil, sol::nil, sol::nil, sol::nil, sol::nil));
		}

		if (wiimoteRemoves[i]) {
			events.push_back(std::make_tuple(sol::make_object(s, "wiimotedisconnected"), sol::make_object(s, i), sol::nil, sol::nil, sol::nil, sol::nil, sol::nil));
		}
	}
}
std::tuple<sol::object, sol::object, sol::object, sol::object, sol::object, sol::object, sol::object> poll() {
	std::tuple<sol::object, sol::object, sol::object, sol::object, sol::object, sol::object, sol::object> event = events.back();

	events.pop_back();

	return event;
}

// State functions
void quit() {
	preQuit();

	// Exit
	std::exit(0);
}

} // module
} // event
} // love
