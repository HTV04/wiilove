/* WiiLÖVE graphics module
 *
 * This file is part of WiiLÖVE.
 *
 * Copyright (C) 2022  HTV04
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

// Libraries
#include <grrlib.h>
#include "../lib/FreeTypeGX/FreeTypeGX.h"
#include <ogc/conf.h>
#include <tuple>
#include <string>
#include <stdexcept>
#include <cstdint>

// Classes
#include "../classes/graphics/image.hpp"

// Header
#include "graphics.hpp"

// Data
#include "Vera_ttf.h"

// Local variables
namespace {
	const int width = 640;
	const int height = 480;
	const std::tuple<int, int> dimensions = std::make_tuple(width, height);

	bool widescreen;

	unsigned int color = 0xffffffff; // Default color to white

	FreeTypeGX *fontSystem = new FreeTypeGX(GX_TF_RGBA8);
}

namespace love {
namespace graphics {

void init() {
    // Init GRRLIB
    GRRLIB_Init();

	widescreen = CONF_GetAspectRatio() == CONF_ASPECT_16_9;

	//fontSystem->setCompatibilityMode(FTGX_COMPATIBILITY_DEFAULT_TEVOP_GX_PASSCLR); // GRRLIB compatibility
	fontSystem->loadFont(Vera_ttf, Vera_ttf_size, 12);
}

namespace module {

// Misc. querying functions
std::tuple<int, int> getDimensions() { return dimensions; }
int getHeight() { return height; }
int getWidth() { return width; }
bool isWidescreen() { return widescreen; }

// Set and get drawing color
std::tuple<int, int, int, int> getColor() {
	return std::make_tuple(R(color), G(color), B(color), A(color));
}
void setColor(int r, int g, int b, int a) {
	color = RGBA(r, g, b, a);
}
void setColor1(int r, int g, int b) {
	setColor(r, g, b, 255);
}

// Basic drawing functions
void circle(std::string drawMode, float x, float y, float radius) {
	bool fill;

	if (drawMode.compare("fill") == 0) {
		fill = true;
	} else if (drawMode.compare("line") == 0) {
		fill = false;
	} else {
		throw std::invalid_argument("Invalid DrawMode: " + drawMode);
	}

	GRRLIB_Circle(x, y, radius, color, fill);
}
void line(float x1, float y1, float x2, float y2) {
	GRRLIB_Line(x1, y1, x2, y2, color);
}
void print(std::wstring str, float x, float y, float r, float sx, float sy, float ox, float oy) {
	fontSystem->drawText(x - ox * sx, y - ox * sy, str.c_str(), sx, sy, r, color);
}
void print1(std::wstring str, float x, float y, float r, float sx, float sy) {
	print(str, x, y, r, sx, sy, 0.0, 0.0);
}
void print2(std::wstring str, float x, float y, float r) {
	print(str, x, y, r, 1.0, 1.0, 0.0, 0.0);
}
void print3(std::wstring str, float x, float y) {
	print(str, x, y, 0.0, 1.0, 1.0, 0.0, 0.0);
}
void rectangle(std::string drawMode, float x, float y, float width, float height) {
	bool fill;

	if (drawMode.compare("fill") == 0) {
		fill = true;
	} else if (drawMode.compare("line") == 0) {
		fill = false;
	} else {
		throw std::invalid_argument("Invalid DrawMode: " + drawMode);
	}

	GRRLIB_Rectangle(x, y, width, height, color, fill);
}

// Image drawing functions
void draw(love::graphics::Image image, float x, float y, float r, float sx, float sy, float ox, float oy) {
	GRRLIB_DrawImg(x - ox * sx, y - ox * sx, image.texImg, r, sx, sy, color);
}

// Render
void present() {
	GRRLIB_Render();
}

} // module
} // graphics
} // love
