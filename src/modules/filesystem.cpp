/* WiiLÖVE filesystem module
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
#include <fat.h>
#include <sys/unistd.h>
#include <sys/dir.h>
#include <string>

// Header
#include "filesystem.hpp"

// Data
#include "identity_bin.h"

// Local variables
namespace {
    std::string identity = (const char*) identity_bin;

	std::string identityPath;
}

namespace love {
namespace filesystem {

void init() {
	// If identity is unchanged (set to "|IDENTITY|") then we'll use the default identity "love"
    if (identity.compare("|IDENTITY|") == 0) {
        identity = "love";
    }

	identityPath = "sd:/" + identity + "/data";

	// Create directory if it doesn't exist (but it should already)
	if (chdir(identityPath.c_str())) {
		mkdir(identityPath.c_str(), 0777);
		chdir(identityPath.c_str());
	}
}

} // filesystem
} // love
