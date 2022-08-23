/* WiiLÖVE module
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
#include <string>
#include <tuple>

// Classes
#include "classes/audio/source.hpp"
#include "classes/graphics/font.hpp"
#include "classes/graphics/quad.hpp"
#include "classes/graphics/texture.hpp"

// Modules
#include "modules/audio.hpp"
#include "modules/event.hpp"
#include "modules/filesystem.hpp"
#include "modules/graphics.hpp"
#include "modules/math.hpp"
#include "modules/system.hpp"
#include "modules/timer.hpp"
#if !defined(HW_DOL)
#include "modules/wiimote.hpp"
#endif // !HW_DOL

// Data
#include "love_lua.h"

// Header
#include "love.hpp"

namespace love {

void init(lua_State *s, int argc, char **argv) {
	sol::usertype<love::audio::Source> SourceType;

	sol::usertype<love::graphics::Font> FontType;
	sol::usertype<love::graphics::Quad> QuadType;
	sol::usertype<love::graphics::Texture> TextureType;

	sol::state_view lua(s);

	// Init modules if necessary
	love::filesystem::init(argc, argv);

	love::audio::init();
	love::graphics::init();
	love::timer::init();

#if !defined(HW_DOL)
	love::system::init();
	love::wiimote::init();
#endif // !HW_DOL

	love::event::init();

	// Create love module
	lua["love"] = lua.create_table_with(
		"getMode", love::module::getMode,
		"getVersion", love::module::getVersion,

		"event", lua.create_table_with(
			"pump", love::event::module::pump,
			"poll", love::event::module::poll,

			"quit", love::event::module::quit
		),

		"filesystem", lua.create_table_with(
			"exists", love::filesystem::module::exists,
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
			"setColor", love::graphics::module::setColor,

			"origin", love::graphics::module::origin,
			"pop", love::graphics::module::pop,
			"push", love::graphics::module::push,
			"scale", love::graphics::module::scale,
			"rotate", love::graphics::module::rotate,
			"translate", love::graphics::module::translate,

			"circle", love::graphics::module::circle,
			"line", love::graphics::module::line,
			"rectangle", love::graphics::module::rectangle,

			"getFont", love::graphics::module::getFont,
			"print", love::graphics::module::print,
			"setFont", love::graphics::module::setFont,

			"draw", sol::overload(
				love::graphics::module::draw,
				love::graphics::module::draw1
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
				love::math::module::random1
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
#if !defined(HW_DOL)
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

	// Usertypes setup
	// NOTE: We have to make these in the global namespace due to sol limitations.
	SourceType = lua.new_usertype<love::audio::Source>(
		"_Source", sol::constructors<
			love::audio::Source(const char*, const char*, const char*),
			love::audio::Source(const char*, const char*)
		>(),

		"pause", &love::audio::Source::pause,
		"play", &love::audio::Source::play,
		"stop", &love::audio::Source::stop,

		"getChannelCount", &love::audio::Source::getChannelCount,
		"getPitch", &love::audio::Source::getPitch,
		"getVolume", &love::audio::Source::getVolume,
		"isLooping", &love::audio::Source::isLooping,
		"isPlaying", &love::audio::Source::isPlaying,
		"seek", &love::audio::Source::seek,
		"setLooping", &love::audio::Source::setLooping,
		"setPitch", &love::audio::Source::setPitch,
		"setVolume", &love::audio::Source::setVolume,
		"tell", &love::audio::Source::tell,

		"clone", &love::audio::Source::clone,
		"release", &love::audio::Source::release
	);

	FontType = lua.new_usertype<love::graphics::Font>(
		"_Font", sol::constructors<
			love::graphics::Font(unsigned int),
			love::graphics::Font(),
			love::graphics::Font(const char *, unsigned int),
			love::graphics::Font(const char *)
		>(),

		"clone", &love::graphics::Font::clone,
		"release", &love::graphics::Font::release
	);
	QuadType = lua.new_usertype<love::graphics::Quad>(
		"_Quad", sol::constructors<
			love::graphics::Quad(float, float, float, float, unsigned int, unsigned int),
			love::graphics::Quad(float, float, float, float, const love::graphics::Texture &)
		>(),

		"getTextureDimensions", &love::graphics::Quad::getTextureDimensions,
		"getViewport", &love::graphics::Quad::getViewport,
		"setViewport", &love::graphics::Quad::setViewport,

		"release", &love::graphics::Quad::release
	);
	TextureType = lua.new_usertype<love::graphics::Texture>(
		"_Texture", sol::constructors<love::graphics::Texture(const char *)>(),

		"getWidth", &love::graphics::Texture::getWidth,
		"getHeight", &love::graphics::Texture::getHeight,
		"getDimensions", &love::graphics::Texture::getDimensions,

		"clone", &love::graphics::Texture::clone,
		"release", &love::graphics::Texture::release
	);

	// Lua-based API
	lua.require_script("love", std::string(love_lua, love_lua + love_lua_size), false, "WiiLÖVE Lua API", sol::load_mode::text);
}

// Perform pre-exit tasks
void quit() {
	// Be a good boy, clear the memory allocated by GRRLIB
	GRRLIB_Exit();
}

namespace module {

std::string getMode() {
#if defined(WIILOVE_MODE_FINAL)
	return "final";
#elif defined(WIILOVE_MODE_DEBUG) // WIILOVE_MODE_FINAL
	return "debug";
#else // WIILOVE_MODE_DEBUG
	return "release";
#endif // !WIILOVE_MODE_DEBUG
}
std::tuple<int, int, int> getVersion() {
	return std::make_tuple(1, 0, 0); // Major, minor, patch
}

} // module
} // love
