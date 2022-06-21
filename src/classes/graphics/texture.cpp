/* WiiLÖVE Texture class
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
#include <sol/sol.hpp> // Prevents macro conflicts
#include <grrlib-mod.h>

// Modules
#include "../../modules/filesystem.hpp"

// Header
#include "texture.hpp"

namespace love {
namespace graphics {

// Constructor
Texture::Texture(std::string filename) {
	texture = GRRLIB_LoadTextureFromFile(love::filesystem::getFilePath(filename).c_str());
}

// Clone constructor
Texture::Texture(int *instances, GRRLIB_texture *texture) {
	this->instances = instances;
	this->texture = texture;
}

// Object functions
Texture *Texture::clone() {
	instances++;

	return new Texture(instances, texture);
}

// Destructor
Texture::~Texture() {
	instances--;
	if (instances == 0) {
		GRRLIB_FreeTexture(texture);

		delete instances;
	}
}

} // graphics
} // love
