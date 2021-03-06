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

#include "ags/engine/debugging/consoleoutputtarget.h"
#include "ags/engine/debugging/debug_log.h"
#include "ags/globals.h"

namespace AGS3 {
namespace AGS {
namespace Engine {

ConsoleOutputTarget::ConsoleOutputTarget() {
}

ConsoleOutputTarget::~ConsoleOutputTarget() {}

void ConsoleOutputTarget::PrintMessage(const DebugMessage &msg) {
	// limit number of characters for console
	// TODO: is there a way to find out how many characters can fit in?
	_G(debug_line)[_G(last_debug_line)] = msg.Text.Left(99);

	_G(last_debug_line) = (_G(last_debug_line) + 1) % DEBUG_CONSOLE_NUMLINES;
	if (_G(last_debug_line) == _G(first_debug_line))
		_G(first_debug_line) = (_G(first_debug_line) + 1) % DEBUG_CONSOLE_NUMLINES;
}

} // namespace Engine
} // namespace AGS
} // namespace AGS3
