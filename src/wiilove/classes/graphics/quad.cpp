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
#include <utility>
#include <tuple>

// Classes
#include "texture.hpp"

// Modules
#include "../../modules/filesystem.hpp"

// Header
#include "quad.hpp"

namespace love {
namespace graphics {

// Constructor
Quad::Quad(float x, float y, float width, float height, unsigned int sw, unsigned int sh) {
	texturePart = GRRLIB_CreateTexturePartEx(x, y, width, height, sw, sh);
}
Quad::Quad(float x, float y, float width, float height, const Texture &texture) : Quad(x, y, width, height, texture.texture->width, texture.texture->height) {}

// Quad properties
std::pair<unsigned int, unsigned int> Quad::getTextureDimensions() {
	return std::make_pair(texturePart->textureWidth, texturePart->textureHeight);
}
std::tuple<float, float, float, float> Quad::getViewport() {
	return std::make_tuple(texturePart->x, texturePart->y, texturePart->width, texturePart->height);
}
void Quad::setViewport(float x, float y, float width, float height) {
	unsigned int sw = texturePart->textureWidth;
	unsigned int sh = texturePart->textureHeight;

	GRRLIB_FreeTexturePart(texturePart);

	texturePart = GRRLIB_CreateTexturePartEx(x, y, width, height, sw, sh);
}

// Object functions
void Quad::release() { delete this; }

// Destructor
Quad::~Quad() {
	GRRLIB_FreeTexturePart(texturePart);
}

} // graphics
} // love
