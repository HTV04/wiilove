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
#include <sol/sol.hpp>
#include <fat.h>
#include <string>
#include <filesystem>
#include <fstream>
#include <cstdlib>

// Header
#include "filesystem.hpp"

namespace love {
namespace filesystem {

void init(int argc, char **argv) {
	std::string executablePath;
	std::string filesystemPath;

	if (fatInitDefault() == false) {
		throw std::runtime_error("Failed to initialize filesystem");
	}

	if (argc > 0) {
		executablePath = argv[0];

		filesystemPath = executablePath.substr(0, executablePath.find_last_of("/"));
	} else {
		// This case is interesting, since this means that the executable is not loaded
		// with any arguments.
		// * We're likely running on Dolphin (directly), because the Homebrew Channel would
		//   at least pass the executable path. Potentially same issue with Wii VC injects?
		// * We'll default to sd:/wiilove as the filesystem path.

		filesystemPath = "sd:/wiilove";
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

std::string getFilePath(const std::string &filename) { // Get the path of a file, using the save directory as a filter and the data directory otherwise.
	std::string savePath = "save/" + filename;

	if (std::filesystem::exists(savePath)) {
		return savePath;
	} else { // If the save path doesn't exist, return the data path.
		return "data/" + filename; // TODO: Add error handling
	}
}
void getFileData(const char *filename, void *&data, int &size) {
	std::string filePath = getFilePath(filename);

	std::ifstream file(filePath, std::ios::binary | std::ios::ate); // TODO: Add error handling

	size = file.tellg();
	data = std::malloc(size);

	file.seekg(0, std::ios::beg);
	file.read(static_cast<char *>(data), size);
}

namespace module {

// Working with files
bool exists(const char *filename) {
	return std::filesystem::exists(getFilePath(filename));
}
std::string read(const char *filename) {
	std::stringstream stream;

	stream << std::ifstream(getFilePath(filename), std::ios::binary).rdbuf(); // TODO: Add error handling

	return stream.str();
}
void write(const std::string &filename, const char *data) {
	std::ofstream out("save/" + filename); // TODO: Add error handling

	out << data; // Write data to file
}

} // module
} // filesystem
} // love
