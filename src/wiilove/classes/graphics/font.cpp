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
#include <FreeTypeGX.h>
#include <iostream>
#include <cstdlib>

// Modules
#include "../../modules/filesystem.hpp"

// Header
#include "font.hpp"

// Data
#include "open-sans_ttf.h"

namespace love {
namespace graphics {

// Local variables
namespace {
	constexpr int defaultFontSize = 12;
}

// Constructors
Font::Font(unsigned int size) { // Load Vera.ttf as default font
	instances = new int(1);
	data = nullptr;
	dataSize = nullptr;
	fontSize = new int(size);

	fontSystem = new FreeTypeGX();

	fontSystem->loadFont(open_sans_ttf, open_sans_ttf_size, *fontSize);
}
Font::Font() : Font(defaultFontSize) {} // Load Vera.ttf as default font (with default size)
Font::Font(const char *filename, unsigned int size) { // Load TTF font
	instances = new int(1);
	dataSize = new int(0);
	fontSize = new int(size);

	fontSystem = new FreeTypeGX();

	filesystem::getFileData(filename, data, *dataSize);
	fontSystem->loadFont(static_cast<unsigned char*>(data), *dataSize, *fontSize);
}
Font::Font(const char *filename) : Font(filename, defaultFontSize) {} // Load TTF font (with default size)

// Clone constructor
Font::Font(const Font &other) {
	instances = other.instances;
	data = other.data;
	dataSize = other.dataSize;
	fontSize = other.fontSize;

	(*instances)++;

	fontSystem = new FreeTypeGX();

	if (data == nullptr)
		fontSystem->loadFont(open_sans_ttf, open_sans_ttf_size, *fontSize);
	else
		fontSystem->loadFont(static_cast<unsigned char *>(data), *dataSize, *fontSize);
}

// Object functions
Font *Font::clone() {
	return new Font(*this);
}
void Font::release() { delete this; }

// Destructor
Font::~Font() {
	delete fontSystem;

	if (--(*instances) == 0) {
		delete fontSize;
		delete dataSize;
		std::free(data);
		delete instances;
	}
}

} // graphics
} // love
