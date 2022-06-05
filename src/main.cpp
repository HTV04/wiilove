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
#ifndef HW_DOL
#include <wiiuse/wpad.h>
#endif // !HW_DOL
#include <string>

// Classes
#include "classes/graphics/font.hpp"
#include "classes/graphics/texture.hpp"

// Modules
#include "love.hpp"
#include "modules/event.hpp"
#include "modules/filesystem.hpp"
#include "modules/graphics.hpp"
#include "modules/math.hpp"
#include "modules/system.hpp"
#include "modules/timer.hpp"
#ifndef HW_DOL
#include "modules/wiimote.hpp"
#endif // !HW_DOL

// Data
#include "api_lua.h"
#include "boot_lua.h"

int main(int argc, char **argv) {
	sol::state lua;

	sol::usertype<love::graphics::Font> FontType;
	sol::usertype<love::graphics::Texture> TextureType;

	// Init GRRLIB
	GRRLIB_Init();

	// Init Lua state with default libraries
	lua.open_libraries(
		sol::lib::base,
		sol::lib::package,
		sol::lib::coroutine,
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

	// Init modules if necessary
	love::filesystem::init(argc, argv);

	love::graphics::init();
	love::timer::init();

#ifndef HW_DOL
	love::system::init();
	love::wiimote::init();
#endif // !HW_DOL

	love::event::init();

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

			"clear", sol::overload(
				love::graphics::module::clear,
				love::graphics::module::clear1,
				love::graphics::module::clear2
			),
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
			"getDeflicker", love::graphics::module::getDeflicker,
			"getLineWidth", love::graphics::module::getLineWidth,
			"getPointSize", love::graphics::module::getPointSize,
			"reset", love::graphics::module::reset,
			"setAntiAliasing", love::graphics::module::setAntiAliasing,
			"setDeflicker", love::graphics::module::setDeflicker,
			"setLineWidth", love::graphics::module::setLineWidth,
			"setPointSize", love::graphics::module::setPointSize,

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
#ifndef HW_DOL
		),

		"wiimote", lua.create_table_with(
			"getAngle", love::wiimote::module::getAngle,
			"getExtension", love::wiimote::module::getExtension,
			"getPosition", love::wiimote::module::getPosition,
			"getX", love::wiimote::module::getX,
			"getY", love::wiimote::module::getY,
			"isConnected", love::wiimote::module::isConnected,
			"isDown", love::wiimote::module::isDown,
			"isRumbling", love::wiimote::module::isRumbling,

			"isClassicDown", love::wiimote::module::isClassicDown,

			"setRumble", love::wiimote::module::setRumble
#endif // !HW_DOL
		)
	);

	FontType = lua.new_usertype<love::graphics::Font>(
		"_Font", sol::constructors<
			love::graphics::Font(unsigned int),
			love::graphics::Font(),
			love::graphics::Font(std::string, unsigned int),
			love::graphics::Font(std::string)
		>(),

		"clone", &love::graphics::Font::clone
	);
	TextureType = lua.new_usertype<love::graphics::Texture>(
		"_Texture", sol::constructors<love::graphics::Texture(std::string)>(),

		"clone", &love::graphics::Texture::clone
	);

	// Lua API expansion
	lua.script(std::string(api_lua, api_lua + api_lua_size));

	// Start!
	lua.script(std::string(boot_lua, boot_lua + boot_lua_size));

	// Quit
	love::event::module::quit();
}
