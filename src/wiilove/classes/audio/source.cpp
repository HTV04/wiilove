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

// Libraries
#include <audiogc.hpp>
#include <string>
#include <map>

// Modules
#include "../../modules/filesystem.hpp"

// Header
#include "source.hpp"

namespace love {
namespace audio {

std::map<std::string, audiogc::type> typeMap = {
	{"flac", audiogc::type::flac},
	{"mp3", audiogc::type::mp3},
	{"ogg", audiogc::type::vorbis},
	{"wav", audiogc::type::wav}
};
std::map<std::string, audiogc::mode> modeMap = {
	{"stream", audiogc::mode::stream},
	{"static", audiogc::mode::store}
};

// Constructors
Source::Source(std::string type, std::string filename, std::string mode) {
	player = new audiogc::player(typeMap[type], filesystem::getFilePath(filename), modeMap[mode]);
}
Source::Source(std::string filename, std::string mode) {
	player = new audiogc::player(audiogc::type::detect, filesystem::getFilePath(filename), modeMap[mode]);
}

// Clone constructor
Source::Source(const Source &other) {
	this->player = new audiogc::player(*other.player);
}

// Playback functions
void Source::pause() { player->pause(); }
void Source::play() { player->play(); }
void Source::stop() { player->stop(); }

// Playback state functions
unsigned char Source::getChannelCount() { return player->get_channel_count(); }
double Source::getPitch() { return player->get_pitch(); }
unsigned char Source::getVolume() { return player->get_volume(); }
bool Source::isLooping() { return player->is_looping(); }
bool Source::isPlaying() { return player->is_playing(); }
void Source::seek(int offset) { player->seek(offset); }
void Source::setLooping(bool loop) { player->set_looping(loop); }
double Source::setPitch(double pitch) { return player->set_pitch(pitch); }
void Source::setVolume(int volume) { player->set_volume(volume); }
double Source::tell() { return player->tell(); }

// Object functions
Source *Source::clone() { return new Source(*this); }

// Destructor
Source::~Source() { delete player; }

} // audio
} // love
