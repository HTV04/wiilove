/* WiiLÖVE Font class
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
#include "../../lib/FreeTypeGX/FreeTypeGX.h"
#include <iostream>
#include <cstdlib>

// Modules
#include "../../modules/filesystem.hpp"

// Header
#include "font.hpp"

// Data
#include "Vera_ttf.h"

#define DEFAULT_SIZE 12

namespace love {
namespace graphics {

// Constructors
Font::Font(unsigned int size) { // Load Vera.ttf as default font
	instances = new int(1);
	data = nullptr;
	dataSize = nullptr;
	fontSize = new int(size);

	fontSystem = new FreeTypeGX();

	fontSystem->loadFont(Vera_ttf, Vera_ttf_size, *fontSize);
}
Font::Font() : Font(DEFAULT_SIZE) {} // Load Vera.ttf as default font (with default size)
Font::Font(std::string filename, unsigned int size) { // Load TTF font
	instances = new int(1);
	dataSize = new int(0);
	fontSize = new int(size);

	fontSystem = new FreeTypeGX();

	filesystem::getFileData(filename, data, *dataSize);
	fontSystem->loadFont(static_cast<unsigned char*>(data), *dataSize, *fontSize);
}
Font::Font(std::string filename) : Font(filename, DEFAULT_SIZE) {} // Load TTF font (with default size)

// Clone constructor
Font::Font(int *instances, void *data, int *dataSize, int *fontSize) {
	this->instances = instances;
	this->data = data;
	this->dataSize = dataSize;
	this->fontSize = fontSize;

	fontSystem = new FreeTypeGX();

	if (data == nullptr)
		fontSystem->loadFont(Vera_ttf, Vera_ttf_size, *fontSize);
	else
		fontSystem->loadFont(static_cast<unsigned char*>(data), *dataSize, *fontSize);
}

// Object functions
Font *Font::clone() {
	instances++;

	return new Font(instances, data, dataSize, fontSize);
}

// Destructor
Font::~Font() {
	delete fontSystem;

	instances--;
	if (instances == 0) {
		delete instances;
		std::free(data);
		delete dataSize;
		delete fontSize;
	}
}

} // graphics
} // love
