/* ScummVM - Scumm Interpreter
 * Copyright (C) 2003 The ScummVM project
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * $Header$
 *
 */

#include "stdafx.h"
#include "queen/music.h"
#include "queen/queen.h"
#include "queen/resource.h"

#include "sound/mididrv.h"
#include "sound/midiparser.h"

namespace Queen {

	Music::Music(MidiDriver *driver, QueenEngine *vm) : _loop(false), _driver(driver) {
		_midi = MidiParser::createParser_SMF();
		_midi->setMidiDriver(_driver);
		int ret = _driver->open();
		if (ret)
			warning("MIDI Player init failed: \"%s\"", _driver->getErrorName(ret));
		_midi->setTimerRate(_driver->getBaseTempo());
		_driver->setTimerCallback((void *)_midi, _midi->timerCallback);
					
		_musicData = vm->resource()->loadFile("AQ.RL", 0, NULL);
		_musicDataSize = vm->resource()->fileSize("AQ.RL");
		_numSongs = READ_LE_UINT16(_musicData);
	}

	Music::~Music() {
		stopSong();
		_midi->unloadMusic();
		// Send All Notes Off
		for (int i = 0; i < 16; ++i)
			_driver->send((123 << 8) | 0xB0 | i);
		_driver->close();
		delete _midi;
		delete[] _musicData;	
	}

	void Music::playSong(uint16 songNum) {
		if (_loop)
			_midi->property(MidiParser::mpAutoLoop, 1);
		else
			_midi->property(MidiParser::mpAutoLoop, 0);
		
		_midi->loadMusic(_musicData + songOffset(songNum), songLength(songNum));
		_midi->setTrack(0);		
		_driver->setTimerCallback((void *)_midi, _midi->timerCallback);
	}

	void Music::stopSong() {
		_driver->setTimerCallback(NULL, NULL);
	}
	
	uint32 Music::songOffset(uint16 songNum) {
		uint16 offsLo = READ_LE_UINT16(_musicData + (songNum * 4) + 2);
		uint16 offsHi = READ_LE_UINT16(_musicData + (songNum * 4) + 4);
		return (offsHi << 4) | offsLo;
	}

	uint32 Music::songLength(uint16 songNum) {
		if (songNum < _numSongs)
			return (songOffset(songNum + 1) - songOffset(songNum));
		return (_musicDataSize - songOffset(songNum));
	}
	
} // End of namespace Queen
