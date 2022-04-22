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
#include <grrlib-mod.h>
#include "../lib/FreeTypeGX/FreeTypeGX.h"
#include <ogc/gx.h>
#include <ogc/conf.h>
#include <utility>
#include <vector>
#include <tuple>
#include <string>
#include <stdexcept>
#include <cmath>

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

	std::tuple<unsigned char, unsigned char, unsigned char> backgroundColor {0, 0, 0}; // Default color to black

	love::graphics::Font *curFont; // Initial font
}

namespace love {
namespace graphics {

void init() {
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
void clear(unsigned char r, unsigned char g, unsigned char b) {
	GRRLIB_FillScreen(GRRLIB_RGBA(r, g, b, 255));
}
std::tuple<unsigned char, unsigned char, unsigned char> getBackgroundColor() {
	return backgroundColor;
}
std::tuple<unsigned char, unsigned char, unsigned char, unsigned char> getColor() {
	unsigned long int color = GRRLIB_Settings.color;

	return std::make_tuple(GRRLIB_R(color), GRRLIB_G(color), GRRLIB_B(color), GRRLIB_A(color));
}
void setBackgroundColor(unsigned char r, unsigned char g, unsigned char b) {
	backgroundColor = std::make_tuple(r, g, b); // Since only used for values, optimize by storing as tuple
}
void setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	GRRLIB_Settings.color = GRRLIB_RGBA(r, g, b, a);
}
void setColor1(unsigned char r, unsigned char g, unsigned char b) {
	setColor(r, g, b, 255);
}

// Basic drawing functions
void circle(bool fill, float x, float y, float radius) {
	GRRLIB_Circle(x, y, radius, fill);
}
void line(float x1, float y1, float x2, float y2) {
	GRRLIB_Line(x1, y1, x2, y2);
}
void rectangle(bool fill, float x, float y, float width, float height) {
	GRRLIB_Rectangle(x, y, width, height, fill);
}

// Font functions
void print(std::wstring str, float x, float y, float r, float sx, float sy, float ox, float oy) {
	curFont->fontSystem->drawText(x, y, str.c_str(), sx, sy, ox, oy, r);
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
void setFont(love::graphics::Font *font) {
	curFont = font;
}

// Texture functions
void draw(love::graphics::Texture &image, float x, float y, float r, float sx, float sy, float ox, float oy) {
	GRRLIB_DrawImg(x, y, image.texImg, r, sx, sy, ox, oy);
}
void draw1(love::graphics::Texture &image, float x, float y, float r, float sx, float sy) {
	draw(image, x, y, r, sx, sy, 0.0, 0.0);
}
void draw2(love::graphics::Texture &image, float x, float y, float r) {
	draw(image, x, y, r, 1.0, 1.0, 0.0, 0.0);
}
void draw3(love::graphics::Texture &image, float x, float y) {
	draw(image, x, y, 0.0, 1.0, 1.0, 0.0, 0.0);
}

// Graphics state functions
bool getAntiAliasing() {
	return GRRLIB_Settings.antialias;
}
unsigned char getDeflicker() {
	return GRRLIB_Settings.deflicker;
}
unsigned char getLineWidth() {
	return GRRLIB_Settings.lineWidth;
}
unsigned char getPointSize() {
	return GRRLIB_Settings.pointSize;
}
void reset() {
	GRRLIB_Settings.color = 0xFFFFFFFF;
	backgroundColor = std::make_tuple(0, 0, 0);

	origin();

	GRRLIB_Settings.antialias = true;
	GRRLIB_SetDeflicker(false);
	GRRLIB_SetLineWidth(1);
	GRRLIB_SetPointSize(1);
}
void setAntiAliasing(bool enable) {
	GRRLIB_Settings.antialias = enable;
}
void setDeflicker(bool enable) {
	GRRLIB_SetDeflicker(enable);
}
void setLineWidth(unsigned char width) {
	GRRLIB_SetLineWidth(width);
}
void setPointSize(unsigned char size) {
	GRRLIB_SetPointSize(size);
}

// Rendering functions
void present() {
	GRRLIB_Render();
}

} // module
} // graphics
} // love
