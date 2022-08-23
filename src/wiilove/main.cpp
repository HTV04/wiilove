/* WiiLÖVE 1.0.0 alpha 4
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
#include <string>

// Modules
#include "love.hpp"

// Data
#include "boot_lua.h"

int main(int argc, char **argv) {
	sol::state lua;

	// Init Lua state with default libraries
	lua.open_libraries(
		sol::lib::base,
		sol::lib::package,
		sol::lib::string,
		sol::lib::os,
		sol::lib::math,
		sol::lib::table,
		sol::lib::debug,
		sol::lib::bit32,
		sol::lib::io,
		sol::lib::ffi,
		sol::lib::jit
	);

	// Init WiiLÖVE API
	love::init(lua, argc, argv);

	// Start!
	lua.script(std::string(boot_lua, boot_lua + boot_lua_size), "WiiLÖVE Boot", sol::load_mode::text);

	// Quit
	love::quit();

	std::exit(0);
}
