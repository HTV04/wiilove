/* WiiLÖVE wiimote module
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

#pragma once

// Libraries
#include "../lib/sol.hpp"
#include <vector>

// Classes
#include "../classes/wiimote/wiimoteClass.hpp"

namespace love {
namespace wiimote {

void init();
void update(std::vector<bool> &adds, std::vector<bool> &removes, int &homePressed);

namespace module {

sol::table getWiimotes(sol::this_state s);

} // module
} // system
} // love