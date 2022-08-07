/* WiiLÖVE math module
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
#include <wiiuse/wpad.h>
#include <random>

// Header
#include "math.hpp"

namespace love {
namespace math {

std::default_random_engine randomGenerator(gettime());

std::uniform_real_distribution<double> randomDoubleDist(0.0, 1.0);

namespace module {

// Random number generation
int random(int min, int max) {
    std::uniform_int_distribution<int> randomIntDist(min, max);

    return randomIntDist(randomGenerator);
};
int random1(int max) {
    return random(1, max);
};
double random2() {
    return randomDoubleDist(randomGenerator);
};

} // module
} // math
} // love
