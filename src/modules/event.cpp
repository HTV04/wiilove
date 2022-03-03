/* WiiLÖVE event module
 *
 * This file is part of WiiLÖVE.
 *
 * Copyright (C) 2022  HTV04
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
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
}

namespace love {
namespace event {
namespace module {

// Event functions
// NOTE: We have to add events in reverse!
void pump(sol::this_state s) {
    std::vector<bool> wiimoteAdds {false, false, false, false};
    std::vector<bool> wiimoteRemoves {false, false, false, false};

    int homePressed = -1;

    events.push_back(std::make_tuple(sol::nil, sol::nil, sol::nil, sol::nil, sol::nil, sol::nil, sol::nil));

    // Update wiimotes
    love::wiimote::update(wiimoteAdds, wiimoteRemoves, homePressed);

    if (homePressed != -1) {
        events.push_back(std::make_tuple(sol::make_object(s, "homepressed"), sol::make_object(s, homePressed), sol::nil, sol::nil, sol::nil, sol::nil, sol::nil));
    }
    for (int i = 3; i > -1; i--) {
        if (wiimoteAdds[i]) {
            events.push_back(std::make_tuple(sol::make_object(s, "wiimoteadded"), sol::make_object(s, i), sol::nil, sol::nil, sol::nil, sol::nil, sol::nil));
        }

        if (wiimoteRemoves[i]) {
            events.push_back(std::make_tuple(sol::make_object(s, "wiimoteremoved"), sol::make_object(s, i), sol::nil, sol::nil, sol::nil, sol::nil, sol::nil));
        }
    }
}
std::tuple<sol::object, sol::object, sol::object, sol::object, sol::object, sol::object, sol::object> getPoll() {
    std::tuple<sol::object, sol::object, sol::object, sol::object, sol::object, sol::object, sol::object> event = events.back();

    events.pop_back();

    return event;
}

// State functions
void quit() {
    // Be a good boy, clear the memory allocated by GRRLIB
    GRRLIB_Exit();

    // Exit
    std::exit(0);
}

} // module
} // event
} // love
