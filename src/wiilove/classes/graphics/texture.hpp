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

#pragma once

#include <grrlib-mod.h>
#include <utility>

namespace love {
namespace graphics {

class Texture {
	private:
		int *instances;

	public:
		GRRLIB_texture *texture;

		Texture(const char *filename);

		Texture(const Texture &other);

		int getWidth();
		int getHeight();
		std::pair<int, int> getDimensions();

		Texture *clone();
		void release();

		~Texture();
};

} // graphics
} // love
