/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef LASTEXPRESS_CLOCK_H
#define LASTEXPRESS_CLOCK_H

#include "common/scummsys.h"

namespace LastExpress {

class LastExpressEngine;
class SequenceFrame;

class Clock {
public:
	explicit Clock(LastExpressEngine *engine);
	~Clock();

	void draw(uint32 time);
	void clear();

private:
	LastExpressEngine *_engine;

	// Frames
	SequenceFrame *_frameMinutes;
	SequenceFrame *_frameHour;
	SequenceFrame *_frameSun;
	SequenceFrame *_frameDate;
};

} // End of namespace LastExpress

#endif // LASTEXPRESS_CLOCK_H
