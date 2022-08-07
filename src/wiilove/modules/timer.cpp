/* WiiLÖVE timer module
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
#include <ogc/lwp_watchdog.h>
#include <unistd.h>

// Header
#include "timer.hpp"

namespace love {
namespace timer {

unsigned long long lastTime;
double deltaTime = 0.0;

unsigned long long lastFrameTime;
double fps = 0.0;
double avgDelta = 0.0;
double frames = 0.0;

void init() {
	lastTime = gettime();

	lastFrameTime = lastTime;
}

namespace module {

// Querying functions
double getAverageDelta() { return avgDelta; }
double getDelta() { return deltaTime; }
double getFPS() { return fps; }
double getTime() { return ticks_to_millisecs(gettime()); }

// Actions
void sleep(int ms) { usleep(ms); }
double step() { // Update timer
	unsigned long long curTime = gettime();

	double sinceFrameTime = static_cast<double>(curTime - lastFrameTime) / static_cast<double>(TB_TIMER_CLOCK * 1000);

	// Update delta time
	deltaTime = static_cast<double>(curTime - lastTime) / static_cast<double>(TB_TIMER_CLOCK * 1000);

	// Update FPS and average delta
	frames++;
	if(sinceFrameTime >= 1.0) {
		fps = frames;
		avgDelta = sinceFrameTime / frames;

		frames = 0.0;
		lastFrameTime = curTime;
	}

	lastTime = curTime;

	return deltaTime;
}

} // module
} // timer
} // love
