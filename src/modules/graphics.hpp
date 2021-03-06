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

#pragma once

// Libraries
#include <tuple>
#include <string>

// Classes
#include "../classes/graphics/font.hpp"

namespace love {
namespace graphics {

void init();

namespace module {

std::pair<int, int> getDimensions();
int getHeight();
int getWidth();
bool isWidescreen();

void clear(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void clear1(unsigned char r, unsigned char g, unsigned char b);
void clear2();
std::tuple<unsigned char, unsigned char, unsigned char, unsigned char> getBackgroundColor();
std::tuple<unsigned char, unsigned char, unsigned char, unsigned char> getColor();
void setBackgroundColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void setColor1(unsigned char r, unsigned char g, unsigned char b);

void origin();
void pop();
void push();
void scale(float x, float y);
void rotate(float angle);
void translate(float dx, float dy);

void circle(bool fill, float x, float y, float radius);
void line(float x1, float y1, float x2, float y2);
void rectangle(bool fill, float x, float y, float width, float height);

void print(std::wstring str, float x, float y, float r, float sx, float sy, float ox, float oy);
void print1(std::wstring str, float x, float y, float r, float sx, float sy);
void print2(std::wstring str, float x, float y, float r);
void print3(std::wstring str, float x, float y);
void setFont(love::graphics::Font *font);

void draw(love::graphics::Texture &image, float x, float y, float r, float sx, float sy, float ox, float oy);
void draw1(love::graphics::Texture &image, float x, float y, float r, float sx, float sy);
void draw2(love::graphics::Texture &image, float x, float y, float r);
void draw3(love::graphics::Texture &image, float x, float y);

bool getAntiAliasing();
unsigned char getDeflicker();
unsigned char getLineWidth();
unsigned char getPointSize();
void reset();
void setAntiAliasing(bool enable);
void setDeflicker(bool enable);
void setLineWidth(unsigned char width);
void setPointSize(unsigned char size);

void present();

} // module
} // graphics
} // love
