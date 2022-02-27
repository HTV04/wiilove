/* WiiLÖVE 1.0 "Cumulus" alpha 1
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

#define SOL_ALL_SAFETIES_ON 1 // Recommended

// Libraries
#include "lib/sol.hpp"
#include <grrlib.h>
#include <wiiuse/wpad.h>
#include <string>

// Classes
#include "classes/graphics/image.hpp"
#include "classes/wiimote/wiimoteClass.hpp"

// Modules
#include "love.hpp"
#include "modules/event.hpp"
#include "modules/graphics.hpp"
#include "modules/math.hpp"
#include "modules/system.hpp"
#include "modules/timer.hpp"
#include "modules/wiimote.hpp"

// Data
#include "boot_lua.h"

int main(int argc, char **argv) {
    // Init Lua state
    sol::state lua;
    lua.open_libraries(
        sol::lib::base,
        sol::lib::package,
        sol::lib::coroutine,
        sol::lib::string,
        sol::lib::os,
        sol::lib::math,
        sol::lib::table,
        sol::lib::debug
    );

    // Init modules if necessary (and in order of dependency)
    love::graphics::init();
    love::system::init();
    love::timer::init();
    love::wiimote::init();

    // Create love module
	lua["love"] = lua.create_table_with(
		"getVersion", love::module::getVersion,

        "event", lua.create_table_with(
            "pump", love::event::module::pump,
            "getPoll", love::event::module::getPoll,

            "quit", love::event::module::quit
        ),

        "graphics", lua.create_table_with(
            "getDimensions", love::graphics::module::getDimensions,
            "getHeight", love::graphics::module::getHeight,
            "getWidth", love::graphics::module::getWidth,
            "isWidescreen", love::graphics::module::isWidescreen,

            "getColor", love::graphics::module::getColor,
            "setColor", sol::overload(love::graphics::module::setColor, love::graphics::module::setColor1),

            "circle", love::graphics::module::circle,
            "line", love::graphics::module::line,
            "print", love::graphics::module::print,
            "rectangle", love::graphics::module::rectangle,

            "draw", love::graphics::module::draw,

            "present", love::graphics::module::present
        ),

		"math", lua.create_table_with(
            "random", sol::overload(love::math::module::doubleRandom, love::math::module::intRandom)
        ),

        "system", lua.create_table_with(
            "getConsole", love::system::module::getConsole,
            "getLanguage", love::system::module::getLanguage,
            "getNickname", love::system::module::getNickname,

            "scanPads", love::system::module::scanPads
        ),

        "timer", lua.create_table_with(
            "step", love::timer::module::step,

            "sleep", love::timer::module::sleep
        ),

        "wiimote", lua.create_table_with(
            "getWiimotes", love::wiimote::module::getWiimotes
        )
	);

    // Usertypes setup
    // NOTE: We have to make these in the global namespace due to sol limitations.
    //       For now, we'll work around this in boot.lua.
    sol::usertype<love::graphics::Image> Image_type = lua.new_usertype<love::graphics::Image>(
        "_Image", sol::constructors<love::graphics::Image(const char*)>()
    );

    sol::usertype<love::wiimote::Wiimote> Wiimote_type = lua.new_usertype<love::wiimote::Wiimote>(
        "_Wiimote", sol::constructors<love::wiimote::Wiimote()>()
    );

    lua["_Wiimote"]["getExtension"] = &love::wiimote::Wiimote::getExtension;
    lua["_Wiimote"]["getX"] = &love::wiimote::Wiimote::getX;
    lua["_Wiimote"]["getY"] = &love::wiimote::Wiimote::getY;
    lua["_Wiimote"]["isConnected"] = &love::wiimote::Wiimote::isConnected;
    lua["_Wiimote"]["isDown"] = &love::wiimote::Wiimote::isDown;
    lua["_Wiimote"]["isRumbling"] = &love::wiimote::Wiimote::isRumbling;

    lua["_Wiimote"]["isClassicDown"] = &love::wiimote::Wiimote::isClassicDown;

    lua["_Wiimote"]["rumble"] = &love::wiimote::Wiimote::rumble;
    lua["_Wiimote"]["rumbleDuration"] = &love::wiimote::Wiimote::rumbleDuration;

    // Workaround for global usertypes
    lua["love"]["graphics"]["newImage"] = lua["_Image"]["new"];

    // Delete global usertypes
    lua["_Image"] = sol::nil;
    lua["_Wiimote"] = sol::nil;

    // Start!
    lua.script(std::string(boot_lua, boot_lua + boot_lua_size));

    // Quit
    love::event::module::quit();
}
