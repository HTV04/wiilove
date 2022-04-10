/* WiiLÖVE system module
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
#include <ogc/conf.h>
#include <wiiuse/wpad.h>
#include <string>
#include <map>
#include <cstdlib>

// Header
#include "system.hpp"

// Local variables
namespace {
	std::map<int, std::string> languageMap = {
		{CONF_LANG_JAPANESE, "Japanese"},
		{CONF_LANG_ENGLISH, "English"},
		{CONF_LANG_GERMAN, "German"},
		{CONF_LANG_FRENCH, "French"},
		{CONF_LANG_SPANISH, "Spanish"},
		{CONF_LANG_ITALIAN, "Italian"},
		{CONF_LANG_DUTCH, "Dutch"},
		{CONF_LANG_SIMP_CHINESE, "Simplified Chinese"},
		{CONF_LANG_TRAD_CHINESE, "Traditional Chinese"},
		{CONF_LANG_KOREAN, "Korean"}
	};

	std::string language;
	std::string nickname;
}

namespace love {
namespace system {

void init() {
	int languageInt = CONF_GetLanguage();
	uint8_t nicknameInt[22];

	// Cache language
	if (languageMap.count(languageInt) > 0) {
		language = languageMap[languageInt];
	} else {
		language = "Unknown";
	}

	// Cache nickname
	CONF_GetNickName(nicknameInt);
	nickname = std::string(nicknameInt, nicknameInt + 22);
}

namespace module {

// Querying functions
std::string getConsole() { return "Wii"; } // Only Wii is supported... for now ;)
std::string getLanguage() { return language; }
std::string getNickname() { return nickname; }

} // module
} // system
} // love
