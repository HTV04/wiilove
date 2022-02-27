/* WiiLÖVE timer module
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
#include <ogc/lwp_watchdog.h>
#include <sys/unistd.h>
#include <cstdint>

// Header
#include "timer.hpp"

// Local variables
namespace {
    std::uint64_t deltaTimeStart;
    double deltaTime;
}

namespace love {
namespace timer {

void init() {
    settime((std::uint64_t)0); // So we don't have to start with a huge number

    deltaTimeStart = gettime();
}

namespace module {

// Querying functions
double getDelta() { return deltaTime; }

// Actions
void sleep(int ms) {
    usleep(ms);
}
double step() { // Update deltaTime
    deltaTime = (double) (gettime() - deltaTimeStart) / (double) (TB_TIMER_CLOCK * 1000); // Division is to convert from ticks to seconds
    deltaTimeStart = gettime();

    return deltaTime;
}

} // module
} // timer
} // love
