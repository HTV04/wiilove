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
#include <dr_wav.h>
#include <aesndlib.h>
#include <ogc/lwp.h>
#include <string>

namespace love {
namespace audio {

class Source {
	private:
		int *instances;
		std::string *filename;

		void init();

	public:
		drwav *wav;

		void *buffer;
		int framesPerBuffer;
		int position = 0;

		AESNDPB *aesndPb;

		float pitch;
		int volume;
		bool playing;
		bool paused;

		lwpq_t threadQueue;
		lwp_t thread;

		Source(std::string filename);

		Source(int *instances, std::string *filename);

		void pause();
		void play();
		void stop();

		float getPitch();
		unsigned char getVolume();
		bool isPaused();
		bool isPlaying();
		bool isStopped();
		void seek(int offset);
		void setPitch(float pitch);
		void setVolume(int volume);
		double tell();

		Source *clone();

		~Source();
};

} // graphics
} // love
