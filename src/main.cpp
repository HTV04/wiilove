/* WiiLÖVE 1.0 "Cumulus" alpha 1
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

#define SOL_ALL_SAFETIES_ON 1 // Recommended

// Libraries
#include "lib/sol.hpp"
#include <grrlib.h>
#include <wiiuse/wpad.h>
#include <string>

// Classes
#include "classes/graphics/font.hpp"
#include "classes/graphics/texture.hpp"
#include "classes/wiimote/wiimote-class.hpp"

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
            "setColor", sol::overload(
                love::graphics::module::setColor,
                love::graphics::module::setColor1
            ),

            "origin", love::graphics::module::origin,
            "pop", love::graphics::module::pop,
            "push", love::graphics::module::push,
            "scale", love::graphics::module::scale,
            "rotate", love::graphics::module::rotate,
            "translate", love::graphics::module::translate,

            "circle", love::graphics::module::circle,
            "line", love::graphics::module::line,
            "rectangle", love::graphics::module::rectangle,

            "print", sol::overload(
                love::graphics::module::print,
                love::graphics::module::print1,
                love::graphics::module::print2,
                love::graphics::module::print3
            ),
            "setFont", love::graphics::module::setFont,

            "draw", love::graphics::module::draw,

            "present", love::graphics::module::present
        ),

		"math", lua.create_table_with(
            "random", sol::overload(
                love::math::module::doubleRandom,
                love::math::module::intRandom
            )
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
    //       For now, we'll work around this.
    sol::usertype<love::graphics::Font> FontType = lua.new_usertype<love::graphics::Font>(
        "_Font", sol::constructors<
            love::graphics::Font(unsigned int),
            love::graphics::Font(),
            love::graphics::Font(const char*, unsigned int),
            love::graphics::Font(const char*)
        >()
    );

    sol::usertype<love::graphics::Texture> TextureType = lua.new_usertype<love::graphics::Texture>(
        "_Texture", sol::constructors<love::graphics::Texture(const char*)>()
    );

    sol::usertype<love::wiimote::Wiimote> WiimoteType = lua.new_usertype<love::wiimote::Wiimote>(
        "_Wiimote", sol::constructors<love::wiimote::Wiimote()>()
    );

    WiimoteType["getExtension"] = &love::wiimote::Wiimote::getExtension;
    WiimoteType["getX"] = &love::wiimote::Wiimote::getX;
    WiimoteType["getY"] = &love::wiimote::Wiimote::getY;
    WiimoteType["isConnected"] = &love::wiimote::Wiimote::isConnected;
    WiimoteType["isDown"] = &love::wiimote::Wiimote::isDown;
    WiimoteType["isRumbling"] = &love::wiimote::Wiimote::isRumbling;

    WiimoteType["isClassicDown"] = &love::wiimote::Wiimote::isClassicDown;

    WiimoteType["rumble"] = &love::wiimote::Wiimote::rumble;
    WiimoteType["rumbleDuration"] = &love::wiimote::Wiimote::rumbleDuration;

    // Workaround for global usertypes
    lua["love"]["graphics"]["newFont"] = lua["_Font"]["new"];
    lua["love"]["graphics"]["newTexture"] = lua["_Texture"]["new"];

    // Delete global usertypes
    lua["_Font"] = sol::nil;
    lua["_Texture"] = sol::nil;
    lua["_Wiimote"] = sol::nil;

    // Start!
    lua.script(std::string(boot_lua, boot_lua + boot_lua_size));

    // Quit
    love::event::module::quit();
}
