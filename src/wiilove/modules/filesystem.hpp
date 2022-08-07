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

#pragma once

// Libraries
#include <sol/sol.hpp>
#include <string>

namespace love {
namespace filesystem {

void init(int argc, char **argv);

std::string getFilePath(std::string filename);
void getFileData(std::string filename, void *&data, int &size);

namespace module {

bool exists(std::string filename);
sol::protected_function load(std::string filename, sol::this_state s);
std::string read(std::string filename);
void write (std::string filename, std::string data);

} // module
} // filesystem
} // love
