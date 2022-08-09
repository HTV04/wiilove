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
#include <sol/sol.hpp>
#include <grrlib-mod.h>
#ifndef HW_DOL
#include <wiiuse/wpad.h>
#endif // !HW_DOL
#include <vector>
#include <tuple>
#include <cstdlib>

// Modules
#include "wiimote.hpp"

// Header
#include "event.hpp"

namespace love {
namespace event {

std::vector<std::tuple<sol::object, sol::object, sol::object, sol::object, sol::object, sol::object, sol::object>> events;

void preQuit() { // Perform pre-quit tasks
	// Be a good boy, clear the memory allocated by GRRLIB
	GRRLIB_Exit();
}

int resetFunc = -1;
void resetPressed(unsigned int irq, void *ctx) { resetFunc = SYS_RETURNTOMENU; }
void powerPressed() { resetFunc = SYS_POWEROFF; }
#ifndef HW_DOL
void wiimotePowerPressed(int id) { resetFunc = SYS_POWEROFF; }
#endif // !HW_DOL

void init () {
	// SYS_ResetSystem callbacks
	SYS_SetResetCallback(resetPressed);
	SYS_SetPowerCallback(powerPressed);
#ifndef HW_DOL
	WPAD_SetPowerButtonCallback(wiimotePowerPressed);
#endif // !HW_DOL
}

eventValues makeEvent(lua_State *s = nullptr, const char *eventName = nullptr, sol::object value1 = sol::nil, sol::object value2 = sol::nil, sol::object value3 = sol::nil, sol::object value4 = sol::nil, sol::object value5 = sol::nil, sol::object value6 = sol::nil) {
	if ((s == nullptr) || (eventName == nullptr))
		return std::make_tuple(sol::nil, sol::nil, sol::nil, sol::nil, sol::nil, sol::nil, sol::nil); // "Nil event"
	else
		return std::make_tuple(sol::make_object(s, eventName), value1, value2, value3, value4, value5, value6);
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

	events.push_back(makeEvent());

#ifndef HW_DOL
	// Update wiimotes
	love::wiimote::update(wiimoteAdds, wiimoteRemoves, homePressed);
#endif // !HW_DOL

	if (homePressed != -1) {
		events.push_back(makeEvent(s, "homepressed", sol::make_object(s, homePressed)));
	}
	for (int i = 3; i >= 0; i--) {
		if (wiimoteAdds[i] == true) {
			events.push_back(makeEvent(s, "wiimoteconnected", sol::make_object(s, i)));
		}

		if (wiimoteRemoves[i] == true) {
			events.push_back(makeEvent(s, "wiimotedisconnected", sol::make_object(s, i)));
		}
	}
}
eventValues poll() {
	eventValues event = events.back();

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
