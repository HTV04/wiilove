/* WiiLÖVE math module
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
#include <wiiuse/wpad.h>
#include <random>

// Header
#include "timer.hpp"

namespace {
    std::random_device rd;
    std::default_random_engine generator(rd());

    std::uniform_real_distribution<double> doubleDist(0.0, 1.0);
}

namespace love {
namespace math {
namespace module {

// Random number generation
double doubleRandom() {
    return doubleDist(generator);
};
int intRandom(int min, int max) {
    std::uniform_int_distribution<int> intDist(min, max);

    return intDist(generator);
};

} // module
} // math
} // love
