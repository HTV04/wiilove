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
#include <string>
#include <filesystem>
#include <fstream>

// Header
#include "filesystem.hpp"

// Local variables
namespace {
    std::string filesystemPath;
}

namespace love {
namespace filesystem {

void init(int argc) {
	std::string executablePath;

	if (argc > 0) { // argv[0] is the executable path
		filesystemPath = executablePath.substr(0, executablePath.find_last_of("/"));
	} else {
		// This case is interesting, since this means that the executable is not loaded
		// with any arguments.
		// * We're likely running on Dolphin (directly), because the Homebrew Channel would
		//   at least pass the executable path. Potentially same issue with Wii VC injects?
		// * We'll default to sd:/love as the filesystem path.

		filesystemPath = "sd:/love";
	}

	// Create filesystem directory if it doesn't exist (but it should already) and
	// enter it
	if (!std::filesystem::exists(filesystemPath)) {
		std::filesystem::create_directory(filesystemPath);
	}
	std::filesystem::current_path(filesystemPath);

	// Create data and save directories if they don't exist (but data should already)
	if (!std::filesystem::exists("data")) {
		std::filesystem::create_directory("data");
	}
	if (!std::filesystem::exists("save")) {
		std::filesystem::create_directory("save");
	}
}

std::string getFilePath(std::string filename) { // Get the path of a file, using the save directory as a filter and the data directory otherwise.
	std::string savePath = "save/" + filename;
	std::string dataPath = "data/" + filename;

	if (std::filesystem::exists(savePath)) {
		return savePath;
	} else { // If the save path doesn't exist, return the data path.
		return dataPath;
	}
}

namespace module {

// Working with files
bool exists(std::string filename) {
	return std::filesystem::exists(getFilePath(filename));
}
sol::protected_function load(std::string filename, sol::this_state s) {
	sol::state_view lua(s);

	// TODO: Add error handling
	return lua.load_file(getFilePath(filename)).get<sol::protected_function>();
}
std::string read(std::string filename) {
	std::stringstream stream;

	// TODO: Add error handling
	stream << std::ifstream(getFilePath(filename)).rdbuf();

	return stream.str();
}
void write(std::string filename, std::string data) {
	// TODO: Add error handling
	std::ofstream out("save/" + filename);

	out << data; // Write data to file
}

} // module
} // filesystem
} // love
