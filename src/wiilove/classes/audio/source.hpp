/* WiiLÖVE Source class
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
#include <audiogc.hpp>

namespace love {
namespace audio {

class Source {
	private:
		audiogc::player *player;

	public:
		Source(const char *type, const char *filename, const char *mode);
		Source(const char *filename, const char *mode);

		Source(const Source &other);

		void pause();
		void play();
		void stop();

		unsigned char getChannelCount();
		double getPitch();
		unsigned char getVolume();
		bool isLooping();
		bool isPlaying();
		void seek(int offset);
		void setLooping(bool loop);
		double setPitch(double pitch);
		void setVolume(int volume);
		double tell();

		Source *clone();
		void release();

		~Source();
};

} // graphics
} // love
