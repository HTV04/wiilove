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

std::string getFilePath(const std::string &filename);
void getFileData(const char *filename, void *&data, int &size);

namespace module {

bool exists(const char *filename);
std::string read(const char *filename);
void write (const std::string &filename, const char *data);

} // module
} // filesystem
} // love
