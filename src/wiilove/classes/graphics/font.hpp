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

// Libraries
#include <FreeTypeGX.hpp>

namespace love {
namespace graphics {

class Font {
	private:
		int *instances;
		void *data;
		int *dataSize;
		int *fontSize;

	public:
		FreeTypeGX *fontSystem;

		Font(unsigned int size);
		Font();
		Font(const char *filename, unsigned int size);
		Font(const char *filename);

		Font(const Font &other);

		Font *clone();
		void release();

		~Font();
};

} // graphics
} // love
