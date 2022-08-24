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

#pragma once

// Libraries
#include <sol/sol.hpp>
#include <tuple>

namespace love {
namespace event {

typedef std::tuple<sol::object, sol::object, sol::object, sol::object, sol::object, sol::object, sol::object> values;

void init();

namespace module {

void pump(sol::this_state s);
values poll();
void push(sol::object eventName, sol::object value1, sol::object value2, sol::object value3, sol::object value4, sol::object value5, sol::object value6, sol::this_state s);
void quit(sol::this_state s);

} // module
} // event
} // love
