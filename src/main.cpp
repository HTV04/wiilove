/* WiiLÖVE 1.0.0 "Terra" alpha 2
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
#include "lib/sol.hpp"
#include <grrlib-mod.h>
#include <wiiuse/wpad.h>
#include <string>

// Classes
#include "classes/graphics/font.hpp"
#include "classes/graphics/texture.hpp"
#include "classes/wiimote/wiimote-class.hpp"

// Modules
#include "love.hpp"
#include "modules/event.hpp"
#include "modules/filesystem.hpp"
#include "modules/graphics.hpp"
#include "modules/math.hpp"
#include "modules/system.hpp"
#include "modules/timer.hpp"
#include "modules/wiimote.hpp"

// Data
#include "boot_lua.h"

int main(int argc, char **argv) {
	// Init GRRLIB
	GRRLIB_Init();

	// Init WPAD
	WPAD_Init();
	WPAD_SetVRes(0, 640, 480);
	WPAD_SetDataFormat(WPAD_CHAN_ALL, WPAD_FMT_BTNS_ACC_IR);

	// Init Lua state with default libraries
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

	// Init modules if necessary
	love::filesystem::init(argc, argv); // A lot of things depend on this

	love::event::init();
	love::graphics::init();
	love::system::init();
	love::timer::init();
	love::wiimote::init();

	// Create love module
	lua["love"] = lua.create_table_with(
		"getVersion", love::module::getVersion,

		"event", lua.create_table_with(
			"pump", love::event::module::pump,
			"poll", love::event::module::poll,

			"quit", love::event::module::quit
		),

		"filesystem", lua.create_table_with(
			"exists", love::filesystem::module::exists,
			"load", love::filesystem::module::load,
			"read", love::filesystem::module::read,
			"write", love::filesystem::module::write
		),

		"graphics", lua.create_table_with(
			"getDimensions", love::graphics::module::getDimensions,
			"getHeight", love::graphics::module::getHeight,
			"getWidth", love::graphics::module::getWidth,
			"isWidescreen", love::graphics::module::isWidescreen,

			"clear", love::graphics::module::clear,
			"getBackgroundColor", love::graphics::module::getBackgroundColor,
			"getColor", love::graphics::module::getColor,
			"setBackgroundColor", love::graphics::module::setBackgroundColor,
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

			"draw", sol::overload(
				love::graphics::module::draw,
				love::graphics::module::draw1,
				love::graphics::module::draw2,
				love::graphics::module::draw3
			),

			"getAntiAliasing", love::graphics::module::getAntiAliasing,
			"reset", love::graphics::module::reset,
			"setAntiAliasing", love::graphics::module::setAntiAliasing,

			"present", love::graphics::module::present
		),

		"math", lua.create_table_with(
			"random", sol::overload(
				love::math::module::random,
				love::math::module::random1,
				love::math::module::random2
			)
		),

		"system", lua.create_table_with(
			"getConsole", love::system::module::getConsole,
			"getLanguage", love::system::module::getLanguage,
			"getNickname", love::system::module::getNickname
		),

		"timer", lua.create_table_with(
			"getAverageDelta", love::timer::module::getAverageDelta,
			"getDelta", love::timer::module::getDelta,
			"getFPS", love::timer::module::getFPS,
			"getTime", love::timer::module::getTime,

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
			love::graphics::Font(std::string, unsigned int),
			love::graphics::Font(std::string)
		>()
	);

	sol::usertype<love::graphics::Texture> TextureType = lua.new_usertype<love::graphics::Texture>(
		"_Texture", sol::constructors<love::graphics::Texture(std::string)>()
	);

	sol::usertype<love::wiimote::Wiimote> WiimoteType = lua.new_usertype<love::wiimote::Wiimote>(
		"_Wiimote", sol::constructors<love::wiimote::Wiimote()>()
	);

	WiimoteType["getAngle"] = &love::wiimote::Wiimote::getAngle;
	WiimoteType["getExtension"] = &love::wiimote::Wiimote::getExtension;
	WiimoteType["getID"] = &love::wiimote::Wiimote::getID;
	WiimoteType["getPosition"] = &love::wiimote::Wiimote::getPosition;
	WiimoteType["getX"] = &love::wiimote::Wiimote::getX;
	WiimoteType["getY"] = &love::wiimote::Wiimote::getY;
	WiimoteType["isConnected"] = &love::wiimote::Wiimote::isConnected;
	WiimoteType["isDown"] = &love::wiimote::Wiimote::isDown;
	WiimoteType["isRumbling"] = &love::wiimote::Wiimote::isRumbling;

	WiimoteType["isClassicDown"] = &love::wiimote::Wiimote::isClassicDown;

	WiimoteType["setRumble"] = &love::wiimote::Wiimote::setRumble;

	// Start!
	lua.script(std::string(boot_lua, boot_lua + boot_lua_size));

	// Quit
	love::event::module::quit();
}
