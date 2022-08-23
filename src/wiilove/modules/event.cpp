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
#if !defined(HW_DOL)
#include <wiiuse/wpad.h>
#endif // !HW_DOL
#include <vector>
#include <tuple>
#include <cstdlib>

// Modules
#include "love.hpp"
#include "wiimote.hpp"

// Header
#include "event.hpp"

namespace love {
namespace event {

// Local variables
namespace {
	std::vector<values> events;

	std::vector<values>::iterator it = events.end();

	int resetFunc = -1;
	void resetPressed(unsigned int irq, void *ctx) { resetFunc = SYS_RETURNTOMENU; }
	void powerPressed() { resetFunc = SYS_POWEROFF; }
#if !defined(HW_DOL)
	void wiimotePowerPressed(int id) { resetFunc = SYS_POWEROFF; }
#endif // !HW_DOL
}

void init () {
	// SYS_ResetSystem callbacks
	SYS_SetResetCallback(resetPressed);
	SYS_SetPowerCallback(powerPressed);
#if !defined(HW_DOL)
	WPAD_SetPowerButtonCallback(wiimotePowerPressed);
#endif // !HW_DOL
}

void pushEvent(lua_State *s, const char *eventName, sol::object value1 = sol::nil, sol::object value2 = sol::nil, sol::object value3 = sol::nil, sol::object value4 = sol::nil, sol::object value5 = sol::nil, sol::object value6 = sol::nil) {
	events.push_back(std::make_tuple(sol::make_object(s, eventName), value1, value2, value3, value4, value5, value6));
}

namespace module {

// Event functions, aka WiiLÖVE's "master update"
void pump(sol::this_state s) {
	std::vector<bool> wiimoteAdds = {false, false, false, false};
	std::vector<bool> wiimoteRemoves = {false, false, false, false};

	int homePressed = -1;

	// Check if resetFunc has a value
	if (resetFunc > -1) { // Run reset function
		love::quit();

		// NOTE: Games won't be able to detect if a hardware button is pressed and prevent its
		//       function. This is by design because it functions as a failsafe in case there is
		//       no other way to exit a game. Developers shouldn't rely on this because unsaved
		//       data will be lost!
		SYS_ResetSystem(resetFunc, 0, 0);
	}

#if !defined(HW_DOL)
	// Update wiimotes
	love::wiimote::update(wiimoteAdds, wiimoteRemoves, homePressed);
#endif // !HW_DOL

	for (int i = 0; i <= 3; i++) {
		if (wiimoteAdds[i] == true) {
			pushEvent(s, "wiimoteconnected", sol::make_object(s, i));
		}

		if (wiimoteRemoves[i] == true) {
			pushEvent(s, "wiimotedisconnected", sol::make_object(s, i));
		}
	}

	if (homePressed != -1) {
		pushEvent(s, "homepressed", sol::make_object(s, homePressed));
	}

	it = events.begin();
}
values poll() {
	if (it == events.end()) {
		events.clear();

		it = events.end();

		return std::make_tuple(sol::nil, sol::nil, sol::nil, sol::nil, sol::nil, sol::nil, sol::nil); // "Nil event"
	} else {
		return *it++;
	}
}
void push(sol::object eventName, sol::object value1, sol::object value2, sol::object value3, sol::object value4, sol::object value5, sol::object value6, sol::this_state s) {
	events.push_back(std::make_tuple(eventName, value1, value2, value3, value4, value5, value6));
}
void quit(sol::this_state s) { pushEvent(s, "quit"); }

} // module
} // event
} // love
