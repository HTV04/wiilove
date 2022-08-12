/* WiiLÖVE Quad class
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
#include <grrlib-mod.h>
#include <utility>
#include <tuple>

// Classes
#include "texture.hpp"

namespace love {
namespace graphics {

class Quad {
	public:
		GRRLIB_texturePart *texturePart;

		Quad(float x, float y, float width, float height, unsigned int sw, unsigned int sh);
		Quad(float x, float y, float width, float height, const Texture &texture);

		std::pair<unsigned int, unsigned int> getTextureDimensions();
		std::tuple<float, float, float, float> getViewport();
		void setViewport(float x, float y, float width, float height);

		void release();

		~Quad();
};

} // graphics
} // love
