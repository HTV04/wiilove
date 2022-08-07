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

#define DR_WAV_IMPLEMENTATION

// Libraries
#include <dr_wav.h>
#include <aesndlib.h>
#include <ogc/cache.h>
#include <ogc/lwp.h>
#include <map>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include <malloc.h>

// Modules
#include "../../modules/filesystem.hpp"

// Header
#include "source.hpp"

namespace love {
namespace audio {

constexpr int bufferSize = 5760;

void aesndCallback(AESNDPB *pb, unsigned int state, void *cbArg) {
	if (state == VOICE_STATE_STREAM) {
		love::audio::Source *source = static_cast<love::audio::Source *>(cbArg);

		AESND_SetVoiceBuffer(pb, source->buffer, bufferSize);

		if (source->playing)
			LWP_ThreadSignal(source->threadQueue);
	}
}

void *wavPlayer(void *arg) {
	love::audio::Source *source = static_cast<love::audio::Source *>(arg);

	while (source->playing) {
		if (drwav_read_pcm_frames_s16be(source->wav, source->framesPerBuffer, static_cast<short *>(source->buffer)) == 0) {
			source->position = 0;

			AESND_SetVoiceStream(source->aesndPb, false);
			AESND_SetVoiceStop(source->aesndPb, true);

			source->playing = false;
			source->paused = false;
		} else {
			DCFlushRange(source->buffer, bufferSize);

			source->position += source->framesPerBuffer;

			LWP_ThreadSleep(source->threadQueue);
		}
	}

	return 0;
}

// Internal functions
void Source::init() {
	int voiceFormat;

	wav = new drwav;

	drwav_init_file(wav, filename->c_str(), NULL);

	buffer = memalign(32, bufferSize);
	framesPerBuffer = bufferSize / (wav->channels * sizeof(short));

	aesndPb = AESND_AllocateVoice(aesndCallback, this);

	switch(wav->channels) {
		case 1:
			voiceFormat = VOICE_MONO16;
			break;
		case 2:
			voiceFormat = VOICE_STEREO16;
			break;
		default:
			throw std::runtime_error("Unsupported number of channels");
	}
	AESND_SetVoiceFormat(aesndPb, voiceFormat);
	setPitch(1.0);
	setVolume(255);
}

// Constructors
Source::Source(std::string filename) {
	instances = new int(1);
	this->filename = new std::string(filesystem::getFilePath(filename));

	init();
}

// Clone constructor
Source::Source(int *instances, std::string *filename) {
	this->instances = instances;
	this->filename = filename;

	init();
}

// Playback functions
void Source::pause() {
	if (playing == true) {
		AESND_SetVoiceStop(aesndPb, true);

		paused = true;
	}
}
void Source::play() {
	if (paused == true) {
		AESND_SetVoiceStop(aesndPb, false);

		paused = false;
	} else if (playing == false) {
		seek(0);

		playing = true;

		LWP_InitQueue(&threadQueue);
		LWP_CreateThread(&thread, wavPlayer, this, NULL, 0, 80);

		AESND_SetVoiceStream(aesndPb, true);
		AESND_SetVoiceStop(aesndPb, false);
	}
}
void Source::stop() {
	position = 0;

	AESND_SetVoiceStream(aesndPb, false);
	AESND_SetVoiceStop(aesndPb, true);

	playing = false;
	paused = false;

	LWP_ThreadSignal(threadQueue); // Give the thread a chance to exit
	LWP_JoinThread(thread, NULL);
}

// Playback state functions
float Source::getPitch() { return pitch; }
unsigned char Source::getVolume() { return volume; }
bool Source::isPaused() { return paused; }
bool Source::isPlaying() { return (playing == true) && (paused == false); }
bool Source::isStopped() { return !playing; }
void Source::seek(int offset) {
	int wasPlaying = playing;
	int wasPaused = paused;

	position = offset * wav->sampleRate;

	if (wasPlaying == true)
		stop();

	drwav_seek_to_pcm_frame(wav, position);

	if (wasPlaying == true) {
		if (wasPaused == false)
			play();
		else
			pause();
	}
}
void Source::setPitch(float pitch) {
	this->pitch = pitch;

	AESND_SetVoiceFrequency(aesndPb, wav->sampleRate * pitch);
}
void Source::setVolume(int volume) {
	// Despite being a short, the safe volume range is 0-255
	if (volume > 255)
		volume = 255;
	else if (volume < 0)
		volume = 0;

	this->volume = volume;

	AESND_SetVoiceVolume(aesndPb, volume, volume);
}
double Source::tell() { return static_cast<double>(position) / wav->sampleRate; }

// Object functions
Source *Source::clone() {
	instances++;

	Source *source = new Source(instances, filename);

	source->setPitch(pitch);
	source->setVolume(volume);

	return source;
}

// Destructor
Source::~Source() {
	stop();

	AESND_FreeVoice(aesndPb);

	free(buffer);

	delete wav;

	instances--;
	if (instances == 0) {
		delete filename;
		delete instances;
	}
}

} // audio
} // love
