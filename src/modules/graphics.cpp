/* WiiLÖVE graphics module
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
#include "../lib/FreeTypeGX/FreeTypeGX.h"
#include <ogc/gu.h>
#include <ogc/conf.h>
#include <utility>
#include <vector>
#include <tuple>
#include <string>
#include <stdexcept>

// Classes
#include "../classes/graphics/font.hpp"
#include "../classes/graphics/texture.hpp"

// Header
#include "graphics.hpp"

// Local variables
namespace {
	const int width = 640;
	const int height = 480;
	const std::pair<int, int> dimensions = std::make_pair(width, height);

	bool widescreen;

	// Transforms are stored for push/pop operations
	std::vector<GRRLIB_matrix> transforms;

	std::tuple<int, int, int> backgroundColor {0, 0, 0}; // Default color to black
	unsigned int color = 0xffffffff; // Default color to white

	love::graphics::Font *curFont; // Initial font
}

namespace love {
namespace graphics {

void init() {
    // Init GRRLIB
    GRRLIB_Init();

	widescreen = CONF_GetAspectRatio() == CONF_ASPECT_16_9;

	curFont = new love::graphics::Font();
}

namespace module {

// Misc. querying functions
std::pair<int, int> getDimensions() { return dimensions; }
int getHeight() { return height; }
int getWidth() { return width; }
bool isWidescreen() { return widescreen; }

// Transformation functions
void origin() {
	if (transforms.empty()) { return; } // No transforms performed

	GRRLIB_Origin();
}
void pop() {
	if (transforms.empty()) { throw std::runtime_error("Stack is empty"); }

	GRRLIB_SetMatrix(&transforms.back()); // Use previously stored transform before removing it

	transforms.pop_back();
}
void push() {
	transforms.push_back(GRRLIB_GetMatrix()); // Store the current transform for later
}
void rotate(float angle) {
	GRRLIB_Rotate(angle);
}
void scale(float x, float y) {
	GRRLIB_Scale(x, y);
}
void translate(float dx, float dy) {
	GRRLIB_Translate(dx, dy);
}

// Set and get drawing colors
void clear(int r, int g, int b) {
	GRRLIB_FillScreen(RGBA(r, g, b, 255));
}
std::tuple<int, int, int> getBackgroundColor() {
	return backgroundColor;
}
std::tuple<int, int, int, int> getColor() {
	return std::make_tuple(R(color), G(color), B(color), A(color));
}
void setBackgroundColor(int r, int g, int b) {
	backgroundColor = std::make_tuple(r, g, b); // Since only used for values, optimize by storing as tuple
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

// Font functions
void print(std::wstring str, float x, float y, float r, float sx, float sy, float ox, float oy) {
	curFont->fontSystem->drawText(x - ox * sx, y - ox * sy, str.c_str(), sx, sy, r, color);
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
void setFont(love::graphics::Font &font) {
	curFont = &font;
}

// Texture functions
void draw(love::graphics::Texture &image, float x, float y, float r, float sx, float sy, float ox, float oy) {
	GRRLIB_DrawImg(x - ox * sx, y - ox * sx, image.texImg, r, sx, sy, color);
}

// Render
void present() {
	GRRLIB_Render();
}

} // module
} // graphics
} // love
