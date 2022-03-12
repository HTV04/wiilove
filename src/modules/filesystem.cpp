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
#include "../lib/sol.hpp"
#include <sys/unistd.h>
#include <dirent.h>
#include <string>
#include <fstream>
#include <cstdio>

// Header
#include "filesystem.hpp"

// Local variables
namespace {
    std::string filesystemPath;
}

namespace love {
namespace filesystem {

void init(int argc, char **argv) {
	std::string executablePath;

	if (argc > 0) { // argv[0] is the executable path
		filesystemPath = executablePath.substr(0, executablePath.find_last_of("/"));
	} else {
		// This case is interesting, since this means that the executable is not loaded with any arguments.
		// * We're likely running on Dolphin (directly), because the Homebrew Channel would at least pass the executable path. Potentially same issue with Wii VC injects?
		// * We'll default to sd:/love as the filesystem path.

		filesystemPath = "sd:/love";
	}

	// Create filesystem directory if it doesn't exist (but it should already) and enter it
	if (chdir(filesystemPath.c_str())) {
		mkdir(filesystemPath.c_str(), 0777);
		chdir(filesystemPath.c_str());
	}

	// Create data and save directories if they don't exist (but data should already)
	if (opendir("data") == NULL) {
		mkdir("data", 0777);
	}
	if (opendir("save") == NULL) {
		mkdir("save", 0777);
	}
}

std::string getFilePath(std::string filename) { // Get the path of a file, using the save directory as a filter and the data directory otherwise.
	std::string savePath = "save/" + filename;
	std::string dataPath = "data/" + filename;

	FILE *f = fopen(savePath.c_str(), "r");

	if (f != NULL) {
		fclose(f);

		return savePath;
	} else { // If the save path doesn't exist, return the data path.
		return dataPath;
	}
}

namespace module {

// Working with files
sol::protected_function load(std::string filename, sol::this_state s) {
	sol::state_view lua(s);

	// TODO: Add error handling
	return lua.load_file(getFilePath(filename).c_str()).get<sol::protected_function>();
}
std::string read(std::string filename) {
	std::stringstream stream;

	// TODO: Add error handling
	stream << std::ifstream(getFilePath(filename).c_str()).rdbuf();

	return stream.str();
}

} // module
} // filesystem
} // love
