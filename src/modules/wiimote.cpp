/* WiiLÖVE wiimote module
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
#include "../lib/sol.hpp"
#include <ogc/conf.h>
#include <wiiuse/wpad.h>
#include <vector>
#include <utility>

// Classes
#include "../classes/wiimote/wiimote-class.hpp"

// Header
#include "system.hpp"

// Local variables
namespace {
    std::vector<love::wiimote::Wiimote> wiimotes(4);
}

namespace love {
namespace wiimote {

void init() {
    int homePressed = -1;

    // Init WPAD module
    WPAD_Init();
    WPAD_SetVRes(0, 640, 480);
    WPAD_SetDataFormat(WPAD_CHAN_ALL, WPAD_FMT_BTNS_ACC_IR);

    for (int i = 0; i < 4; i++) {
        Wiimote &wiimote = wiimotes[i];

        wiimote.id = i;

        wiimote.update(homePressed); // Init values
    }
}
void update(std::vector<bool> &adds, std::vector<bool> &removes, int &homePressed) {
    WPAD_ScanPads();

    for (int i = 0; i < 4; i++) {
        Wiimote &wiimote = wiimotes[i];

        int status = WPAD_Probe(i, NULL);
        if (wiimote.isConnected() and status != WPAD_ERR_NONE) {
            removes[i] = true;
        } else if (!wiimote.isConnected() and status == WPAD_ERR_NONE) {
            adds[i] = true;
        }

        wiimote.update(homePressed);
    }
}

namespace module {

sol::table getWiimotes(sol::this_state s) {
    sol::table wiimoteTable(s, sol::create);

    for (int i = 0; i < 4; i++) {
        wiimoteTable[i + 1] = &wiimotes[i];
    }

    return wiimoteTable;
}

} // module
} // wiimote
} // love
