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
#include <grrlib.h>
#include "../../lib/FreeTypeGX/FreeTypeGX.h"
#include <cstdlib>

// Header
#include "font.hpp"

// Data
#include "Vera_ttf.h"

#define defaultSize 12

namespace love {
namespace graphics {

// Internal functions
Font::Font(unsigned int size) { // Load Vera.ttf as default font
	fontSystem = new FreeTypeGX();

	fontSystem->loadFont(Vera_ttf, Vera_ttf_size, size);
}
Font::Font() : Font(defaultSize) {} // Load Vera.ttf as default font (with default size)
Font::Font(const char *filename, unsigned int size) { // Load TTF font
	uint8_t *data;
	int dataSize;

	fontSystem = new FreeTypeGX();

	dataSize = GRRLIB_LoadFile(filename, &data);
	fontSystem->loadFont(data, dataSize, size);
}
Font::Font(const char *filename) : Font(filename, defaultSize) {} // Load TTF font (with default size)

} // graphics
} // love
