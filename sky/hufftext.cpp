/* ScummVM - Scumm Interpreter
 * Copyright (C) 2002-2006 The ScummVM project
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $Header$
 *
 */

#include "text.h"

namespace Sky {

#ifdef PALMOS_68K
const HuffTree *Text::_huffTree_00109;
const HuffTree *Text::_huffTree_00267;
const HuffTree *Text::_huffTree_00288;
const HuffTree *Text::_huffTree_00303;
const HuffTree *Text::_huffTree_00331;
const HuffTree *Text::_huffTree_00348;
const HuffTree *Text::_huffTree_00365;
const HuffTree *Text::_huffTree_00368;
const HuffTree *Text::_huffTree_00372;
#else
const HuffTree Text::_huffTree_00109[] = {
	{ 1, 22, 0 },
	{ 2, 9, 0 },
	{ 3, 6, 0 },
	{ 4, 5, 0 },
	{ 0, 0, 0x20 },
	{ 0, 0, 0x65 },
	{ 7, 8, 0 },
	{ 0, 0, 0x74 },
	{ 0, 0, 0x6F },
	{ 10, 17, 0 },
	{ 11, 14, 0 },
	{ 12, 13, 0 },
	{ 0, 0, 0x2E },
	{ 0, 0, 0x61 },
	{ 15, 16, 0 },
	{ 0, 0, 0x00 },
	{ 0, 0, 0x73 },
	{ 18, 21, 0 },
	{ 19, 20, 0 },
	{ 0, 0, 0x6E },
	{ 0, 0, 0x68 },
	{ 0, 0, 0x69 },
	{ 23, 46, 0 },
	{ 24, 35, 0 },
	{ 25, 30, 0 },
	{ 26, 29, 0 },
	{ 27, 28, 0 },
	{ 0, 0, 0x45 },
	{ 0, 0, 0x49 },
	{ 0, 0, 0x72 },
	{ 31, 34, 0 },
	{ 32, 33, 0 },
	{ 0, 0, 0x54 },
	{ 0, 0, 0x75 },
	{ 0, 0, 0x6D },
	{ 36, 41, 0 },
	{ 37, 40, 0 },
	{ 38, 39, 0 },
	{ 0, 0, 0x6C },
	{ 0, 0, 0x79 },
	{ 0, 0, 0x4F },
	{ 42, 45, 0 },
	{ 43, 44, 0 },
	{ 0, 0, 0x41 },
	{ 0, 0, 0x53 },
	{ 0, 0, 0x4E },
	{ 47, 66, 0 },
	{ 48, 57, 0 },
	{ 49, 54, 0 },
	{ 50, 53, 0 },
	{ 51, 52, 0 },
	{ 0, 0, 0x52 },
	{ 0, 0, 0x64 },
	{ 0, 0, 0x27 },
	{ 55, 56, 0 },
	{ 0, 0, 0x44 },
	{ 0, 0, 0x4C },
	{ 58, 63, 0 },
	{ 59, 62, 0 },
	{ 60, 61, 0 },
	{ 0, 0, 0x67 },
	{ 0, 0, 0x43 },
	{ 0, 0, 0x63 },
	{ 64, 65, 0 },
	{ 0, 0, 0x48 },
	{ 0, 0, 0x21 },
	{ 67, 84, 0 },
	{ 68, 77, 0 },
	{ 69, 74, 0 },
	{ 70, 73, 0 },
	{ 71, 72, 0 },
	{ 0, 0, 0x55 },
	{ 0, 0, 0x77 },
	{ 0, 0, 0x66 },
	{ 75, 76, 0 },
	{ 0, 0, 0x50 },
	{ 0, 0, 0x4D },
	{ 78, 81, 0 },
	{ 79, 80, 0 },
	{ 0, 0, 0x62 },
	{ 0, 0, 0x57 },
	{ 82, 83, 0 },
	{ 0, 0, 0x3F },
	{ 0, 0, 0x47 },
	{ 85, 94, 0 },
	{ 86, 91, 0 },
	{ 87, 90, 0 },
	{ 88, 89, 0 },
	{ 0, 0, 0x59 },
	{ 0, 0, 0x2C },
	{ 0, 0, 0x70 },
	{ 92, 93, 0 },
	{ 0, 0, 0x42 },
	{ 0, 0, 0x6B },
	{ 95, 100, 0 },
	{ 96, 99, 0 },
	{ 97, 98, 0 },
	{ 0, 0, 0x76 },
	{ 0, 0, 0x46 },
	{ 0, 0, 0x4B },
	{ 101, 106, 0 },
	{ 102, 105, 0 },
	{ 103, 104, 0 },
	{ 0, 0, 0x2D },
	{ 0, 0, 0x56 },
	{ 0, 0, 0x4A },
	{ 107, 120, 0 },
	{ 108, 113, 0 },
	{ 109, 112, 0 },
	{ 110, 111, 0 },
	{ 0, 0, 0x3A },
	{ 0, 0, 0x58 },
	{ 0, 0, 0x6A },
	{ 114, 117, 0 },
	{ 115, 116, 0 },
	{ 0, 0, 0x60 },
	{ 0, 0, 0x5A },
	{ 118, 119, 0 },
	{ 0, 0, 0x78 },
	{ 0, 0, 0x30 },
	{ 121, 132, 0 },
	{ 122, 127, 0 },
	{ 123, 126, 0 },
	{ 124, 125, 0 },
	{ 0, 0, 0x32 },
	{ 0, 0, 0x31 },
	{ 0, 0, 0x51 },
	{ 128, 131, 0 },
	{ 129, 130, 0 },
	{ 0, 0, 0x33 },
	{ 0, 0, 0x39 },
	{ 0, 0, 0x71 },
	{ 133, 142, 0 },
	{ 134, 137, 0 },
	{ 135, 136, 0 },
	{ 0, 0, 0x34 },
	{ 0, 0, 0x38 },
	{ 138, 141, 0 },
	{ 139, 140, 0 },
	{ 0, 0, 0x35 },
	{ 0, 0, 0x7A },
	{ 0, 0, 0x26 },
	{ 143, 150, 0 },
	{ 144, 147, 0 },
	{ 145, 146, 0 },
	{ 0, 0, 0x36 },
	{ 0, 0, 0x29 },
	{ 148, 149, 0 },
	{ 0, 0, 0x28 },
	{ 0, 0, 0x37 },
	{ 151, 158, 0 },
	{ 152, 155, 0 },
	{ 153, 154, 0 },
	{ 0, 0, 0x24 },
	{ 0, 0, 0x22 },
	{ 156, 157, 0 },
	{ 0, 0, 0x2F },
	{ 0, 0, 0x3C },
	{ 159, 162, 0 },
	{ 160, 161, 0 },
	{ 0, 0, 0x3E },
	{ 0, 0, 0x25 },
	{ 163, 164, 0 },
	{ 0, 0, 0x23 },
	{ 165, 166, 0 },
	{ 0, 0, 0x5F },
	{ 0, 0, 0x7C }
};

const HuffTree Text::_huffTree_00267[] = {
	{ 1, 20, 0 },
	{ 2, 7, 0 },
	{ 3, 6, 0 },
	{ 4, 5, 0 },
	{ 0, 0, 0x20 },
	{ 0, 0, 0x65 },
	{ 0, 0, 0x73 },
	{ 8, 15, 0 },
	{ 9, 12, 0 },
	{ 10, 11, 0 },
	{ 0, 0, 0x6d },
	{ 0, 0, 0x61 },
	{ 13, 14, 0 },
	{ 0, 0, 0x0 },
	{ 0, 0, 0x6e },
	{ 16, 19, 0 },
	{ 17, 18, 0 },
	{ 0, 0, 0x69 },
	{ 0, 0, 0x74 },
	{ 0, 0, 0x75 },
	{ 21, 44, 0 },
	{ 22, 31, 0 },
	{ 23, 28, 0 },
	{ 24, 27, 0 },
	{ 25, 26, 0 },
	{ 0, 0, 0x2e },
	{ 0, 0, 0x72 },
	{ 0, 0, 0x45 },
	{ 29, 30, 0 },
	{ 0, 0, 0x6f },
	{ 0, 0, 0x68 },
	{ 32, 39, 0 },
	{ 33, 36, 0 },
	{ 34, 35, 0 },
	{ 0, 0, 0x44 },
	{ 0, 0, 0x67 },
	{ 37, 38, 0 },
	{ 0, 0, 0x49 },
	{ 0, 0, 0x79 },
	{ 40, 43, 0 },
	{ 41, 42, 0 },
	{ 0, 0, 0x53 },
	{ 0, 0, 0x64 },
	{ 0, 0, 0x6c },
	{ 45, 64, 0 },
	{ 46, 53, 0 },
	{ 47, 50, 0 },
	{ 48, 49, 0 },
	{ 0, 0, 0x54 },
	{ 0, 0, 0x4e },
	{ 51, 52, 0 },
	{ 0, 0, 0x63 },
	{ 0, 0, 0x52 },
	{ 54, 59, 0 },
	{ 55, 58, 0 },
	{ 56, 57, 0 },
	{ 0, 0, 0x41 },
	{ 0, 0, 0x4f },
	{ 0, 0, 0x4c },
	{ 60, 63, 0 },
	{ 61, 62, 0 },
	{ 0, 0, 0x48 },
	{ 0, 0, 0x43 },
	{ 0, 0, 0x55 },
	{ 65, 84, 0 },
	{ 66, 75, 0 },
	{ 67, 72, 0 },
	{ 68, 71, 0 },
	{ 69, 70, 0 },
	{ 0, 0, 0x62 },
	{ 0, 0, 0x21 },
	{ 0, 0, 0x27 },
	{ 73, 74, 0 },
	{ 0, 0, 0x77 },
	{ 0, 0, 0x66 },
	{ 76, 81, 0 },
	{ 77, 80, 0 },
	{ 78, 79, 0 },
	{ 0, 0, 0x47 },
	{ 0, 0, 0x4d },
	{ 0, 0, 0x2c },
	{ 82, 83, 0 },
	{ 0, 0, 0x57 },
	{ 0, 0, 0x42 },
	{ 85, 98, 0 },
	{ 86, 93, 0 },
	{ 87, 90, 0 },
	{ 88, 89, 0 },
	{ 0, 0, 0x3f },
	{ 0, 0, 0x50 },
	{ 91, 92, 0 },
	{ 0, 0, 0x6b },
	{ 0, 0, 0x70 },
	{ 94, 97, 0 },
	{ 95, 96, 0 },
	{ 0, 0, 0x76 },
	{ 0, 0, 0x46 },
	{ 0, 0, 0x4b },
	{ 99, 110, 0 },
	{ 100, 105, 0 },
	{ 101, 104, 0 },
	{ 102, 103, 0 },
	{ 0, 0, 0x56 },
	{ 0, 0, 0x59 },
	{ 0, 0, 0x7a },
	{ 106, 109, 0 },
	{ 107, 108, 0 },
	{ 0, 0, 0x2d },
	{ 0, 0, 0x23 },
	{ 0, 0, 0x4a },
	{ 111, 124, 0 },
	{ 112, 117, 0 },
	{ 113, 116, 0 },
	{ 114, 115, 0 },
	{ 0, 0, 0x5a },
	{ 0, 0, 0x28 },
	{ 0, 0, 0x29 },
	{ 118, 121, 0 },
	{ 119, 120, 0 },
	{ 0, 0, 0x6a },
	{ 0, 0, 0x2b },
	{ 122, 123, 0 },
	{ 0, 0, 0x2a },
	{ 0, 0, 0x71 },
	{ 125, 138, 0 },
	{ 126, 131, 0 },
	{ 127, 130, 0 },
	{ 128, 129, 0 },
	{ 0, 0, 0x7d },
	{ 0, 0, 0x3a },
	{ 0, 0, 0x60 },
	{ 132, 135, 0 },
	{ 133, 134, 0 },
	{ 0, 0, 0x30 },
	{ 0, 0, 0x32 },
	{ 136, 137, 0 },
	{ 0, 0, 0x31 },
	{ 0, 0, 0x51 },
	{ 139, 150, 0 },
	{ 140, 145, 0 },
	{ 141, 144, 0 },
	{ 142, 143, 0 },
	{ 0, 0, 0x58 },
	{ 0, 0, 0x78 },
	{ 0, 0, 0x33 },
	{ 146, 149, 0 },
	{ 147, 148, 0 },
	{ 0, 0, 0x39 },
	{ 0, 0, 0x34 },
	{ 0, 0, 0x3c },
	{ 151, 160, 0 },
	{ 152, 157, 0 },
	{ 153, 156, 0 },
	{ 154, 155, 0 },
	{ 0, 0, 0x38 },
	{ 0, 0, 0x5d },
	{ 0, 0, 0x3d },
	{ 158, 159, 0 },
	{ 0, 0, 0x26 },
	{ 0, 0, 0x35 },
	{ 161, 168, 0 },
	{ 162, 165, 0 },
	{ 163, 164, 0 },
	{ 0, 0, 0x36 },
	{ 0, 0, 0x2f },
	{ 166, 167, 0 },
	{ 0, 0, 0x37 },
	{ 0, 0, 0x24 },
	{ 169, 174, 0 },
	{ 170, 173, 0 },
	{ 171, 172, 0 },
	{ 0, 0, 0x3e },
	{ 0, 0, 0x25 },
	{ 0, 0, 0x5e },
	{ 175, 180, 0 },
	{ 176, 179, 0 },
	{ 177, 178, 0 },
	{ 0, 0, 0x22 },
	{ 0, 0, 0x40 },
	{ 0, 0, 0x5b },
	{ 181, 184, 0 },
	{ 182, 183, 0 },
	{ 0, 0, 0x5f },
	{ 0, 0, 0x7b },
	{ 185, 186, 0 },
	{ 0, 0, 0x5c },
	{ 0, 0, 0x7c },
};

const HuffTree Text::_huffTree_00288[] = {
	{ 1, 20, 0 },
	{ 2, 7, 0 },
	{ 3, 4, 0 },
	{ 0, 0, 0x20 },
	{ 5, 6, 0 },
	{ 0, 0, 0x65 },
	{ 0, 0, 0x73 },
	{ 8, 15, 0 },
	{ 9, 12, 0 },
	{ 10, 11, 0 },
	{ 0, 0, 0x6e },
	{ 0, 0, 0x69 },
	{ 13, 14, 0 },
	{ 0, 0, 0x74 },
	{ 0, 0, 0x61 },
	{ 16, 19, 0 },
	{ 17, 18, 0 },
	{ 0, 0, 0x2e },
	{ 0, 0, 0x0 },
	{ 0, 0, 0x45 },
	{ 21, 44, 0 },
	{ 22, 31, 0 },
	{ 23, 28, 0 },
	{ 24, 27, 0 },
	{ 25, 26, 0 },
	{ 0, 0, 0x72 },
	{ 0, 0, 0x6f },
	{ 0, 0, 0x75 },
	{ 29, 30, 0 },
	{ 0, 0, 0x6d },
	{ 0, 0, 0x68 },
	{ 32, 39, 0 },
	{ 33, 36, 0 },
	{ 34, 35, 0 },
	{ 0, 0, 0x49 },
	{ 0, 0, 0x6c },
	{ 37, 38, 0 },
	{ 0, 0, 0x53 },
	{ 0, 0, 0x52 },
	{ 40, 43, 0 },
	{ 41, 42, 0 },
	{ 0, 0, 0x64 },
	{ 0, 0, 0x4e },
	{ 0, 0, 0x54 },
	{ 45, 68, 0 },
	{ 46, 55, 0 },
	{ 47, 52, 0 },
	{ 48, 51, 0 },
	{ 49, 50, 0 },
	{ 0, 0, 0x63 },
	{ 0, 0, 0x41 },
	{ 0, 0, 0x4f },
	{ 53, 54, 0 },
	{ 0, 0, 0x44 },
	{ 0, 0, 0x4c },
	{ 56, 63, 0 },
	{ 57, 60, 0 },
	{ 58, 59, 0 },
	{ 0, 0, 0x43 },
	{ 0, 0, 0x67 },
	{ 61, 62, 0 },
	{ 0, 0, 0x55 },
	{ 0, 0, 0x27 },
	{ 64, 67, 0 },
	{ 65, 66, 0 },
	{ 0, 0, 0x79 },
	{ 0, 0, 0x48 },
	{ 0, 0, 0x70 },
	{ 69, 88, 0 },
	{ 70, 79, 0 },
	{ 71, 76, 0 },
	{ 72, 75, 0 },
	{ 73, 74, 0 },
	{ 0, 0, 0x21 },
	{ 0, 0, 0x4d },
	{ 0, 0, 0x62 },
	{ 77, 78, 0 },
	{ 0, 0, 0x76 },
	{ 0, 0, 0x2c },
	{ 80, 85, 0 },
	{ 81, 84, 0 },
	{ 82, 83, 0 },
	{ 0, 0, 0x50 },
	{ 0, 0, 0x66 },
	{ 0, 0, 0x3f },
	{ 86, 87, 0 },
	{ 0, 0, 0x47 },
	{ 0, 0, 0x42 },
	{ 89, 104, 0 },
	{ 90, 97, 0 },
	{ 91, 94, 0 },
	{ 92, 93, 0 },
	{ 0, 0, 0x77 },
	{ 0, 0, 0x57 },
	{ 95, 96, 0 },
	{ 0, 0, 0x46 },
	{ 0, 0, 0x56 },
	{ 98, 101, 0 },
	{ 99, 100, 0 },
	{ 0, 0, 0x6b },
	{ 0, 0, 0x7a },
	{ 102, 103, 0 },
	{ 0, 0, 0x4b },
	{ 0, 0, 0x2d },
	{ 105, 116, 0 },
	{ 106, 111, 0 },
	{ 107, 110, 0 },
	{ 108, 109, 0 },
	{ 0, 0, 0x4a },
	{ 0, 0, 0x2b },
	{ 0, 0, 0x71 },
	{ 112, 115, 0 },
	{ 113, 114, 0 },
	{ 0, 0, 0x59 },
	{ 0, 0, 0x6a },
	{ 0, 0, 0x5a },
	{ 117, 132, 0 },
	{ 118, 125, 0 },
	{ 119, 122, 0 },
	{ 120, 121, 0 },
	{ 0, 0, 0x23 },
	{ 0, 0, 0x51 },
	{ 123, 124, 0 },
	{ 0, 0, 0x28 },
	{ 0, 0, 0x29 },
	{ 126, 129, 0 },
	{ 127, 128, 0 },
	{ 0, 0, 0x3c },
	{ 0, 0, 0x78 },
	{ 130, 131, 0 },
	{ 0, 0, 0x3a },
	{ 0, 0, 0x2a },
	{ 133, 146, 0 },
	{ 134, 141, 0 },
	{ 135, 138, 0 },
	{ 136, 137, 0 },
	{ 0, 0, 0x58 },
	{ 0, 0, 0x7d },
	{ 139, 140, 0 },
	{ 0, 0, 0x3d },
	{ 0, 0, 0x60 },
	{ 142, 145, 0 },
	{ 143, 144, 0 },
	{ 0, 0, 0x5d },
	{ 0, 0, 0x30 },
	{ 0, 0, 0x32 },
	{ 147, 158, 0 },
	{ 148, 153, 0 },
	{ 149, 152, 0 },
	{ 150, 151, 0 },
	{ 0, 0, 0x31 },
	{ 0, 0, 0x33 },
	{ 0, 0, 0x39 },
	{ 154, 157, 0 },
	{ 155, 156, 0 },
	{ 0, 0, 0x2f },
	{ 0, 0, 0x34 },
	{ 0, 0, 0x5e },
	{ 159, 168, 0 },
	{ 160, 165, 0 },
	{ 161, 164, 0 },
	{ 162, 163, 0 },
	{ 0, 0, 0x38 },
	{ 0, 0, 0x3e },
	{ 0, 0, 0x26 },
	{ 166, 167, 0 },
	{ 0, 0, 0x35 },
	{ 0, 0, 0x40 },
	{ 169, 176, 0 },
	{ 170, 173, 0 },
	{ 171, 172, 0 },
	{ 0, 0, 0x36 },
	{ 0, 0, 0x5f },
	{ 174, 175, 0 },
	{ 0, 0, 0x5c },
	{ 0, 0, 0x37 },
	{ 177, 182, 0 },
	{ 178, 181, 0 },
	{ 179, 180, 0 },
	{ 0, 0, 0x5b },
	{ 0, 0, 0x24 },
	{ 0, 0, 0x7b },
	{ 183, 186, 0 },
	{ 184, 185, 0 },
	{ 0, 0, 0x25 },
	{ 0, 0, 0x9 },
	{ 187, 188, 0 },
	{ 0, 0, 0x22 },
	{ 0, 0, 0x7c },
};

const HuffTree Text::_huffTree_00303[] = {
	{ 1, 22, 0 },
	{ 2, 9, 0 },
	{ 3, 6, 0 },
	{ 4, 5, 0 },
	{ 0, 0, 0x20 },
	{ 0, 0, 0x65 },
	{ 7, 8, 0 },
	{ 0, 0, 0x74 },
	{ 0, 0, 0x73 },
	{ 10, 17, 0 },
	{ 11, 14, 0 },
	{ 12, 13, 0 },
	{ 0, 0, 0x6e },
	{ 0, 0, 0x61 },
	{ 15, 16, 0 },
	{ 0, 0, 0x69 },
	{ 0, 0, 0x2e },
	{ 18, 21, 0 },
	{ 19, 20, 0 },
	{ 0, 0, 0x6f },
	{ 0, 0, 0x0 },
	{ 0, 0, 0x45 },
	{ 23, 46, 0 },
	{ 24, 35, 0 },
	{ 25, 30, 0 },
	{ 26, 29, 0 },
	{ 27, 28, 0 },
	{ 0, 0, 0x72 },
	{ 0, 0, 0x75 },
	{ 0, 0, 0x68 },
	{ 31, 34, 0 },
	{ 32, 33, 0 },
	{ 0, 0, 0x49 },
	{ 0, 0, 0x6d },
	{ 0, 0, 0x6c },
	{ 36, 41, 0 },
	{ 37, 40, 0 },
	{ 38, 39, 0 },
	{ 0, 0, 0x54 },
	{ 0, 0, 0x53 },
	{ 0, 0, 0x52 },
	{ 42, 45, 0 },
	{ 43, 44, 0 },
	{ 0, 0, 0x64 },
	{ 0, 0, 0x4e },
	{ 0, 0, 0x41 },
	{ 47, 70, 0 },
	{ 48, 59, 0 },
	{ 49, 54, 0 },
	{ 50, 53, 0 },
	{ 51, 52, 0 },
	{ 0, 0, 0x63 },
	{ 0, 0, 0x4f },
	{ 0, 0, 0x44 },
	{ 55, 58, 0 },
	{ 56, 57, 0 },
	{ 0, 0, 0x4c },
	{ 0, 0, 0x43 },
	{ 0, 0, 0x79 },
	{ 60, 65, 0 },
	{ 61, 64, 0 },
	{ 62, 63, 0 },
	{ 0, 0, 0x67 },
	{ 0, 0, 0x27 },
	{ 0, 0, 0x55 },
	{ 66, 69, 0 },
	{ 67, 68, 0 },
	{ 0, 0, 0x48 },
	{ 0, 0, 0x21 },
	{ 0, 0, 0x70 },
	{ 71, 88, 0 },
	{ 72, 81, 0 },
	{ 73, 78, 0 },
	{ 74, 77, 0 },
	{ 75, 76, 0 },
	{ 0, 0, 0x4d },
	{ 0, 0, 0x62 },
	{ 0, 0, 0x3f },
	{ 79, 80, 0 },
	{ 0, 0, 0x76 },
	{ 0, 0, 0x66 },
	{ 82, 85, 0 },
	{ 83, 84, 0 },
	{ 0, 0, 0x50 },
	{ 0, 0, 0x2c },
	{ 86, 87, 0 },
	{ 0, 0, 0x77 },
	{ 0, 0, 0x47 },
	{ 89, 102, 0 },
	{ 90, 95, 0 },
	{ 91, 94, 0 },
	{ 92, 93, 0 },
	{ 0, 0, 0x42 },
	{ 0, 0, 0x57 },
	{ 0, 0, 0x6b },
	{ 96, 99, 0 },
	{ 97, 98, 0 },
	{ 0, 0, 0x46 },
	{ 0, 0, 0x56 },
	{ 100, 101, 0 },
	{ 0, 0, 0x59 },
	{ 0, 0, 0x4b },
	{ 103, 114, 0 },
	{ 104, 109, 0 },
	{ 105, 108, 0 },
	{ 106, 107, 0 },
	{ 0, 0, 0x2d },
	{ 0, 0, 0x7a },
	{ 0, 0, 0x4a },
	{ 110, 113, 0 },
	{ 111, 112, 0 },
	{ 0, 0, 0x71 },
	{ 0, 0, 0x2b },
	{ 0, 0, 0x6a },
	{ 115, 132, 0 },
	{ 116, 123, 0 },
	{ 117, 120, 0 },
	{ 118, 119, 0 },
	{ 0, 0, 0x5a },
	{ 0, 0, 0x23 },
	{ 121, 122, 0 },
	{ 0, 0, 0x51 },
	{ 0, 0, 0x78 },
	{ 124, 129, 0 },
	{ 125, 128, 0 },
	{ 126, 127, 0 },
	{ 0, 0, 0x3a },
	{ 0, 0, 0x29 },
	{ 0, 0, 0x28 },
	{ 130, 131, 0 },
	{ 0, 0, 0x3c },
	{ 0, 0, 0x58 },
	{ 133, 148, 0 },
	{ 134, 141, 0 },
	{ 135, 138, 0 },
	{ 136, 137, 0 },
	{ 0, 0, 0x2a },
	{ 0, 0, 0x60 },
	{ 139, 140, 0 },
	{ 0, 0, 0x7d },
	{ 0, 0, 0x3d },
	{ 142, 145, 0 },
	{ 143, 144, 0 },
	{ 0, 0, 0x32 },
	{ 0, 0, 0x30 },
	{ 146, 147, 0 },
	{ 0, 0, 0x5d },
	{ 0, 0, 0x31 },
	{ 149, 160, 0 },
	{ 150, 155, 0 },
	{ 151, 154, 0 },
	{ 152, 153, 0 },
	{ 0, 0, 0x7e },
	{ 0, 0, 0x33 },
	{ 0, 0, 0x7f },
	{ 156, 159, 0 },
	{ 157, 158, 0 },
	{ 0, 0, 0x39 },
	{ 0, 0, 0x34 },
	{ 0, 0, 0x2f },
	{ 161, 172, 0 },
	{ 162, 167, 0 },
	{ 163, 166, 0 },
	{ 164, 165, 0 },
	{ 0, 0, 0x38 },
	{ 0, 0, 0x5e },
	{ 0, 0, 0x26 },
	{ 168, 171, 0 },
	{ 169, 170, 0 },
	{ 0, 0, 0x35 },
	{ 0, 0, 0x36 },
	{ 0, 0, 0x3e },
	{ 173, 182, 0 },
	{ 174, 179, 0 },
	{ 175, 178, 0 },
	{ 176, 177, 0 },
	{ 0, 0, 0x40 },
	{ 0, 0, 0x37 },
	{ 0, 0, 0x5f },
	{ 180, 181, 0 },
	{ 0, 0, 0x5c },
	{ 0, 0, 0x24 },
	{ 183, 190, 0 },
	{ 184, 187, 0 },
	{ 185, 186, 0 },
	{ 0, 0, 0x5b },
	{ 0, 0, 0x80 },
	{ 188, 189, 0 },
	{ 0, 0, 0x81 },
	{ 0, 0, 0x22 },
	{ 191, 194, 0 },
	{ 192, 193, 0 },
	{ 0, 0, 0x25 },
	{ 0, 0, 0x82 },
	{ 195, 196, 0 },
	{ 0, 0, 0x7b },
	{ 197, 198, 0 },
	{ 0, 0, 0x9 },
	{ 0, 0, 0x7c },
};

const HuffTree Text::_huffTree_00331[] = {
	{ 1, 20, 0 },
	{ 2, 7, 0 },
	{ 3, 4, 0 },
	{ 0, 0, 0x20 },
	{ 5, 6, 0 },
	{ 0, 0, 0x65 },
	{ 0, 0, 0x61 },
	{ 8, 15, 0 },
	{ 9, 12, 0 },
	{ 10, 11, 0 },
	{ 0, 0, 0x6f },
	{ 0, 0, 0x73 },
	{ 13, 14, 0 },
	{ 0, 0, 0x74 },
	{ 0, 0, 0x6e },
	{ 16, 19, 0 },
	{ 17, 18, 0 },
	{ 0, 0, 0x2e },
	{ 0, 0, 0x69 },
	{ 0, 0, 0x72 },
	{ 21, 44, 0 },
	{ 22, 31, 0 },
	{ 23, 28, 0 },
	{ 24, 27, 0 },
	{ 25, 26, 0 },
	{ 0, 0, 0x0 },
	{ 0, 0, 0x45 },
	{ 0, 0, 0x75 },
	{ 29, 30, 0 },
	{ 0, 0, 0x6d },
	{ 0, 0, 0x41 },
	{ 32, 39, 0 },
	{ 33, 36, 0 },
	{ 34, 35, 0 },
	{ 0, 0, 0x6c },
	{ 0, 0, 0x49 },
	{ 37, 38, 0 },
	{ 0, 0, 0x64 },
	{ 0, 0, 0x52 },
	{ 40, 43, 0 },
	{ 41, 42, 0 },
	{ 0, 0, 0x4e },
	{ 0, 0, 0x53 },
	{ 0, 0, 0x54 },
	{ 45, 68, 0 },
	{ 46, 55, 0 },
	{ 47, 52, 0 },
	{ 48, 51, 0 },
	{ 49, 50, 0 },
	{ 0, 0, 0x4f },
	{ 0, 0, 0x68 },
	{ 0, 0, 0x63 },
	{ 53, 54, 0 },
	{ 0, 0, 0x44 },
	{ 0, 0, 0x67 },
	{ 56, 63, 0 },
	{ 57, 60, 0 },
	{ 58, 59, 0 },
	{ 0, 0, 0x4c },
	{ 0, 0, 0x43 },
	{ 61, 62, 0 },
	{ 0, 0, 0x70 },
	{ 0, 0, 0x55 },
	{ 64, 67, 0 },
	{ 65, 66, 0 },
	{ 0, 0, 0x21 },
	{ 0, 0, 0x79 },
	{ 0, 0, 0x4d },
	{ 69, 88, 0 },
	{ 70, 79, 0 },
	{ 71, 76, 0 },
	{ 72, 75, 0 },
	{ 73, 74, 0 },
	{ 0, 0, 0x50 },
	{ 0, 0, 0x76 },
	{ 0, 0, 0x48 },
	{ 77, 78, 0 },
	{ 0, 0, 0x3f },
	{ 0, 0, 0x62 },
	{ 80, 85, 0 },
	{ 81, 84, 0 },
	{ 82, 83, 0 },
	{ 0, 0, 0x27 },
	{ 0, 0, 0x66 },
	{ 0, 0, 0x2c },
	{ 86, 87, 0 },
	{ 0, 0, 0x47 },
	{ 0, 0, 0x42 },
	{ 89, 108, 0 },
	{ 90, 99, 0 },
	{ 91, 96, 0 },
	{ 92, 95, 0 },
	{ 93, 94, 0 },
	{ 0, 0, 0x56 },
	{ 0, 0, 0x6b },
	{ 0, 0, 0x46 },
	{ 97, 98, 0 },
	{ 0, 0, 0x71 },
	{ 0, 0, 0x2a },
	{ 100, 105, 0 },
	{ 101, 104, 0 },
	{ 102, 103, 0 },
	{ 0, 0, 0x77 },
	{ 0, 0, 0x4b },
	{ 0, 0, 0x2d },
	{ 106, 107, 0 },
	{ 0, 0, 0x57 },
	{ 0, 0, 0x4a },
	{ 109, 126, 0 },
	{ 110, 117, 0 },
	{ 111, 114, 0 },
	{ 112, 113, 0 },
	{ 0, 0, 0x7a },
	{ 0, 0, 0x2b },
	{ 115, 116, 0 },
	{ 0, 0, 0x59 },
	{ 0, 0, 0x6a },
	{ 118, 123, 0 },
	{ 119, 122, 0 },
	{ 120, 121, 0 },
	{ 0, 0, 0x85 },
	{ 0, 0, 0x29 },
	{ 0, 0, 0x51 },
	{ 124, 125, 0 },
	{ 0, 0, 0x5a },
	{ 0, 0, 0x7e },
	{ 127, 148, 0 },
	{ 128, 137, 0 },
	{ 129, 134, 0 },
	{ 130, 133, 0 },
	{ 131, 132, 0 },
	{ 0, 0, 0x8b },
	{ 0, 0, 0x3c },
	{ 0, 0, 0x8a },
	{ 135, 136, 0 },
	{ 0, 0, 0x7f },
	{ 0, 0, 0x3a },
	{ 138, 143, 0 },
	{ 139, 142, 0 },
	{ 140, 141, 0 },
	{ 0, 0, 0x87 },
	{ 0, 0, 0x23 },
	{ 0, 0, 0x78 },
	{ 144, 147, 0 },
	{ 145, 146, 0 },
	{ 0, 0, 0x58 },
	{ 0, 0, 0x91 },
	{ 0, 0, 0x83 },
	{ 149, 168, 0 },
	{ 150, 159, 0 },
	{ 151, 156, 0 },
	{ 152, 155, 0 },
	{ 153, 154, 0 },
	{ 0, 0, 0x88 },
	{ 0, 0, 0x60 },
	{ 0, 0, 0x32 },
	{ 157, 158, 0 },
	{ 0, 0, 0x30 },
	{ 0, 0, 0x31 },
	{ 160, 165, 0 },
	{ 161, 164, 0 },
	{ 162, 163, 0 },
	{ 0, 0, 0x28 },
	{ 0, 0, 0x2f },
	{ 0, 0, 0x5d },
	{ 166, 167, 0 },
	{ 0, 0, 0x3d },
	{ 0, 0, 0x86 },
	{ 169, 184, 0 },
	{ 170, 177, 0 },
	{ 171, 174, 0 },
	{ 172, 173, 0 },
	{ 0, 0, 0x5e },
	{ 0, 0, 0x33 },
	{ 175, 176, 0 },
	{ 0, 0, 0x39 },
	{ 0, 0, 0x34 },
	{ 178, 181, 0 },
	{ 179, 180, 0 },
	{ 0, 0, 0x7d },
	{ 0, 0, 0x38 },
	{ 182, 183, 0 },
	{ 0, 0, 0x5c },
	{ 0, 0, 0x22 },
	{ 185, 198, 0 },
	{ 186, 193, 0 },
	{ 187, 190, 0 },
	{ 188, 189, 0 },
	{ 0, 0, 0x3e },
	{ 0, 0, 0x26 },
	{ 191, 192, 0 },
	{ 0, 0, 0x8d },
	{ 0, 0, 0x7b },
	{ 194, 197, 0 },
	{ 195, 196, 0 },
	{ 0, 0, 0x35 },
	{ 0, 0, 0x36 },
	{ 0, 0, 0x8f },
	{ 199, 210, 0 },
	{ 200, 205, 0 },
	{ 201, 204, 0 },
	{ 202, 203, 0 },
	{ 0, 0, 0x8e },
	{ 0, 0, 0x8c },
	{ 0, 0, 0x37 },
	{ 206, 209, 0 },
	{ 207, 208, 0 },
	{ 0, 0, 0x89 },
	{ 0, 0, 0x24 },
	{ 0, 0, 0x92 },
	{ 211, 218, 0 },
	{ 212, 215, 0 },
	{ 213, 214, 0 },
	{ 0, 0, 0x5b },
	{ 0, 0, 0x80 },
	{ 216, 217, 0 },
	{ 0, 0, 0x81 },
	{ 0, 0, 0x40 },
	{ 219, 222, 0 },
	{ 220, 221, 0 },
	{ 0, 0, 0x5f },
	{ 0, 0, 0x82 },
	{ 223, 224, 0 },
	{ 0, 0, 0x25 },
	{ 225, 226, 0 },
	{ 0, 0, 0x9 },
	{ 227, 228, 0 },
	{ 0, 0, 0x3b },
	{ 0, 0, 0x7c },
};

const HuffTree Text::_huffTree_00348[] = {
	{ 1, 20, 0 },
	{ 2, 7, 0 },
	{ 3, 4, 0 },
	{ 0, 0, 0x20 },
	{ 5, 6, 0 },
	{ 0, 0, 0x65 },
	{ 0, 0, 0x61 },
	{ 8, 15, 0 },
	{ 9, 12, 0 },
	{ 10, 11, 0 },
	{ 0, 0, 0x6F },
	{ 0, 0, 0x73 },
	{ 13, 14, 0 },
	{ 0, 0, 0x74 },
	{ 0, 0, 0x6E },
	{ 16, 19, 0 },
	{ 17, 18, 0 },
	{ 0, 0, 0x2E },
	{ 0, 0, 0x69 },
	{ 0, 0, 0x72 },
	{ 21, 44, 0 },
	{ 22, 31, 0 },
	{ 23, 28, 0 },
	{ 24, 27, 0 },
	{ 25, 26, 0 },
	{ 0, 0, 0x00 },
	{ 0, 0, 0x45 },
	{ 0, 0, 0x75 },
	{ 29, 30, 0 },
	{ 0, 0, 0x6D },
	{ 0, 0, 0x41 },
	{ 32, 39, 0 },
	{ 33, 36, 0 },
	{ 34, 35, 0 },
	{ 0, 0, 0x6C },
	{ 0, 0, 0x49 },
	{ 37, 38, 0 },
	{ 0, 0, 0x64 },
	{ 0, 0, 0x52 },
	{ 40, 43, 0 },
	{ 41, 42, 0 },
	{ 0, 0, 0x4E },
	{ 0, 0, 0x53 },
	{ 0, 0, 0x54 },
	{ 45, 68, 0 },
	{ 46, 55, 0 },
	{ 47, 52, 0 },
	{ 48, 51, 0 },
	{ 49, 50, 0 },
	{ 0, 0, 0x4F },
	{ 0, 0, 0x68 },
	{ 0, 0, 0x63 },
	{ 53, 54, 0 },
	{ 0, 0, 0x44 },
	{ 0, 0, 0x67 },
	{ 56, 63, 0 },
	{ 57, 60, 0 },
	{ 58, 59, 0 },
	{ 0, 0, 0x4C },
	{ 0, 0, 0x43 },
	{ 61, 62, 0 },
	{ 0, 0, 0x70 },
	{ 0, 0, 0x55 },
	{ 64, 67, 0 },
	{ 65, 66, 0 },
	{ 0, 0, 0x21 },
	{ 0, 0, 0x79 },
	{ 0, 0, 0x4D },
	{ 69, 88, 0 },
	{ 70, 79, 0 },
	{ 71, 76, 0 },
	{ 72, 75, 0 },
	{ 73, 74, 0 },
	{ 0, 0, 0x50 },
	{ 0, 0, 0x76 },
	{ 0, 0, 0x48 },
	{ 77, 78, 0 },
	{ 0, 0, 0x3F },
	{ 0, 0, 0x62 },
	{ 80, 85, 0 },
	{ 81, 84, 0 },
	{ 82, 83, 0 },
	{ 0, 0, 0x27 },
	{ 0, 0, 0x66 },
	{ 0, 0, 0x2C },
	{ 86, 87, 0 },
	{ 0, 0, 0x47 },
	{ 0, 0, 0x42 },
	{ 89, 108, 0 },
	{ 90, 99, 0 },
	{ 91, 96, 0 },
	{ 92, 95, 0 },
	{ 93, 94, 0 },
	{ 0, 0, 0x56 },
	{ 0, 0, 0x6B },
	{ 0, 0, 0x46 },
	{ 97, 98, 0 },
	{ 0, 0, 0x71 },
	{ 0, 0, 0x77 },
	{ 100, 105, 0 },
	{ 101, 104, 0 },
	{ 102, 103, 0 },
	{ 0, 0, 0x4B },
	{ 0, 0, 0x2D },
	{ 0, 0, 0x57 },
	{ 106, 107, 0 },
	{ 0, 0, 0x4A },
	{ 0, 0, 0x2A },
	{ 109, 128, 0 },
	{ 110, 117, 0 },
	{ 111, 114, 0 },
	{ 112, 113, 0 },
	{ 0, 0, 0x7A },
	{ 0, 0, 0x59 },
	{ 115, 116, 0 },
	{ 0, 0, 0x6A },
	{ 0, 0, 0x2B },
	{ 118, 123, 0 },
	{ 119, 122, 0 },
	{ 120, 121, 0 },
	{ 0, 0, 0x51 },
	{ 0, 0, 0x29 },
	{ 0, 0, 0x85 },
	{ 124, 127, 0 },
	{ 125, 126, 0 },
	{ 0, 0, 0x5A },
	{ 0, 0, 0x8B },
	{ 0, 0, 0x3C },
	{ 129, 150, 0 },
	{ 130, 139, 0 },
	{ 131, 136, 0 },
	{ 132, 135, 0 },
	{ 133, 134, 0 },
	{ 0, 0, 0x95 },
	{ 0, 0, 0x7E },
	{ 0, 0, 0x8A },
	{ 137, 138, 0 },
	{ 0, 0, 0x87 },
	{ 0, 0, 0x3A },
	{ 140, 145, 0 },
	{ 141, 144, 0 },
	{ 142, 143, 0 },
	{ 0, 0, 0x7F },
	{ 0, 0, 0x5D },
	{ 0, 0, 0x23 },
	{ 146, 149, 0 },
	{ 147, 148, 0 },
	{ 0, 0, 0x78 },
	{ 0, 0, 0x58 },
	{ 0, 0, 0x91 },
	{ 151, 172, 0 },
	{ 152, 163, 0 },
	{ 153, 158, 0 },
	{ 154, 157, 0 },
	{ 155, 156, 0 },
	{ 0, 0, 0x88 },
	{ 0, 0, 0x60 },
	{ 0, 0, 0x32 },
	{ 159, 162, 0 },
	{ 160, 161, 0 },
	{ 0, 0, 0x30 },
	{ 0, 0, 0x83 },
	{ 0, 0, 0x31 },
	{ 164, 169, 0 },
	{ 165, 168, 0 },
	{ 166, 167, 0 },
	{ 0, 0, 0x2F },
	{ 0, 0, 0x28 },
	{ 0, 0, 0x3D },
	{ 170, 171, 0 },
	{ 0, 0, 0x86 },
	{ 0, 0, 0x5E },
	{ 173, 190, 0 },
	{ 174, 181, 0 },
	{ 175, 178, 0 },
	{ 176, 177, 0 },
	{ 0, 0, 0x33 },
	{ 0, 0, 0x39 },
	{ 179, 180, 0 },
	{ 0, 0, 0x98 },
	{ 0, 0, 0x34 },
	{ 182, 187, 0 },
	{ 183, 186, 0 },
	{ 184, 185, 0 },
	{ 0, 0, 0x7D },
	{ 0, 0, 0x38 },
	{ 0, 0, 0x5C },
	{ 188, 189, 0 },
	{ 0, 0, 0x22 },
	{ 0, 0, 0x9B },
	{ 191, 206, 0 },
	{ 192, 199, 0 },
	{ 193, 196, 0 },
	{ 194, 195, 0 },
	{ 0, 0, 0x26 },
	{ 0, 0, 0x8D },
	{ 197, 198, 0 },
	{ 0, 0, 0x35 },
	{ 0, 0, 0x36 },
	{ 200, 203, 0 },
	{ 201, 202, 0 },
	{ 0, 0, 0x92 },
	{ 0, 0, 0x8F },
	{ 204, 205, 0 },
	{ 0, 0, 0x8E },
	{ 0, 0, 0x93 },
	{ 207, 222, 0 },
	{ 208, 215, 0 },
	{ 209, 212, 0 },
	{ 210, 211, 0 },
	{ 0, 0, 0x8C },
	{ 0, 0, 0x37 },
	{ 213, 214, 0 },
	{ 0, 0, 0x99 },
	{ 0, 0, 0x24 },
	{ 216, 219, 0 },
	{ 217, 218, 0 },
	{ 0, 0, 0x80 },
	{ 0, 0, 0x81 },
	{ 220, 221, 0 },
	{ 0, 0, 0x40 },
	{ 0, 0, 0x5B },
	{ 223, 232, 0 },
	{ 224, 229, 0 },
	{ 225, 228, 0 },
	{ 226, 227, 0 },
	{ 0, 0, 0x9A },
	{ 0, 0, 0x5F },
	{ 0, 0, 0x3E },
	{ 230, 231, 0 },
	{ 0, 0, 0x96 },
	{ 0, 0, 0x82 },
	{ 233, 238, 0 },
	{ 234, 237, 0 },
	{ 235, 236, 0 },
	{ 0, 0, 0x25 },
	{ 0, 0, 0x09 },
	{ 0, 0, 0x9C },
	{ 239, 240, 0 },
	{ 0, 0, 0x97 },
	{ 241, 242, 0 },
	{ 0, 0, 0x7B },
	{ 243, 244, 0 },
	{ 0, 0, 0x94 },
	{ 0, 0, 0x7C },
};

const HuffTree Text::_huffTree_00365[] = {
	{ 1, 20, 0 },
	{ 2, 7, 0 },
	{ 3, 4, 0 },
	{ 0, 0, 0x20 },
	{ 5, 6, 0 },
	{ 0, 0, 0x65 },
	{ 0, 0, 0x61 },
	{ 8, 15, 0 },
	{ 9, 12, 0 },
	{ 10, 11, 0 },
	{ 0, 0, 0x6F },
	{ 0, 0, 0x73 },
	{ 13, 14, 0 },
	{ 0, 0, 0x74 },
	{ 0, 0, 0x6E },
	{ 16, 19, 0 },
	{ 17, 18, 0 },
	{ 0, 0, 0x2E },
	{ 0, 0, 0x69 },
	{ 0, 0, 0x72 },
	{ 21, 44, 0 },
	{ 22, 31, 0 },
	{ 23, 28, 0 },
	{ 24, 27, 0 },
	{ 25, 26, 0 },
	{ 0, 0, 0x00 },
	{ 0, 0, 0x45 },
	{ 0, 0, 0x75 },
	{ 29, 30, 0 },
	{ 0, 0, 0x6D },
	{ 0, 0, 0x41 },
	{ 32, 39, 0 },
	{ 33, 36, 0 },
	{ 34, 35, 0 },
	{ 0, 0, 0x6C },
	{ 0, 0, 0x49 },
	{ 37, 38, 0 },
	{ 0, 0, 0x64 },
	{ 0, 0, 0x52 },
	{ 40, 43, 0 },
	{ 41, 42, 0 },
	{ 0, 0, 0x4E },
	{ 0, 0, 0x53 },
	{ 0, 0, 0x54 },
	{ 45, 68, 0 },
	{ 46, 55, 0 },
	{ 47, 52, 0 },
	{ 48, 51, 0 },
	{ 49, 50, 0 },
	{ 0, 0, 0x4F },
	{ 0, 0, 0x68 },
	{ 0, 0, 0x63 },
	{ 53, 54, 0 },
	{ 0, 0, 0x44 },
	{ 0, 0, 0x67 },
	{ 56, 63, 0 },
	{ 57, 60, 0 },
	{ 58, 59, 0 },
	{ 0, 0, 0x4C },
	{ 0, 0, 0x43 },
	{ 61, 62, 0 },
	{ 0, 0, 0x70 },
	{ 0, 0, 0x55 },
	{ 64, 67, 0 },
	{ 65, 66, 0 },
	{ 0, 0, 0x21 },
	{ 0, 0, 0x79 },
	{ 0, 0, 0x4D },
	{ 69, 88, 0 },
	{ 70, 79, 0 },
	{ 71, 76, 0 },
	{ 72, 75, 0 },
	{ 73, 74, 0 },
	{ 0, 0, 0x50 },
	{ 0, 0, 0x76 },
	{ 0, 0, 0x48 },
	{ 77, 78, 0 },
	{ 0, 0, 0x3F },
	{ 0, 0, 0x62 },
	{ 80, 85, 0 },
	{ 81, 84, 0 },
	{ 82, 83, 0 },
	{ 0, 0, 0x27 },
	{ 0, 0, 0x66 },
	{ 0, 0, 0x2C },
	{ 86, 87, 0 },
	{ 0, 0, 0x47 },
	{ 0, 0, 0x42 },
	{ 89, 108, 0 },
	{ 90, 99, 0 },
	{ 91, 96, 0 },
	{ 92, 95, 0 },
	{ 93, 94, 0 },
	{ 0, 0, 0x56 },
	{ 0, 0, 0x6B },
	{ 0, 0, 0x46 },
	{ 97, 98, 0 },
	{ 0, 0, 0x71 },
	{ 0, 0, 0x77 },
	{ 100, 105, 0 },
	{ 101, 104, 0 },
	{ 102, 103, 0 },
	{ 0, 0, 0x4B },
	{ 0, 0, 0x2D },
	{ 0, 0, 0x57 },
	{ 106, 107, 0 },
	{ 0, 0, 0x4A },
	{ 0, 0, 0x2A },
	{ 109, 128, 0 },
	{ 110, 117, 0 },
	{ 111, 114, 0 },
	{ 112, 113, 0 },
	{ 0, 0, 0x7A },
	{ 0, 0, 0x59 },
	{ 115, 116, 0 },
	{ 0, 0, 0x6A },
	{ 0, 0, 0x2B },
	{ 118, 123, 0 },
	{ 119, 122, 0 },
	{ 120, 121, 0 },
	{ 0, 0, 0x51 },
	{ 0, 0, 0x85 },
	{ 0, 0, 0x29 },
	{ 124, 127, 0 },
	{ 125, 126, 0 },
	{ 0, 0, 0x5A },
	{ 0, 0, 0x8B },
	{ 0, 0, 0x3C },
	{ 129, 150, 0 },
	{ 130, 139, 0 },
	{ 131, 136, 0 },
	{ 132, 135, 0 },
	{ 133, 134, 0 },
	{ 0, 0, 0x95 },
	{ 0, 0, 0x7E },
	{ 0, 0, 0x8A },
	{ 137, 138, 0 },
	{ 0, 0, 0x87 },
	{ 0, 0, 0x3A },
	{ 140, 145, 0 },
	{ 141, 144, 0 },
	{ 142, 143, 0 },
	{ 0, 0, 0x7F },
	{ 0, 0, 0x5D },
	{ 0, 0, 0x23 },
	{ 146, 149, 0 },
	{ 147, 148, 0 },
	{ 0, 0, 0x78 },
	{ 0, 0, 0x58 },
	{ 0, 0, 0x91 },
	{ 151, 172, 0 },
	{ 152, 163, 0 },
	{ 153, 158, 0 },
	{ 154, 157, 0 },
	{ 155, 156, 0 },
	{ 0, 0, 0x88 },
	{ 0, 0, 0x60 },
	{ 0, 0, 0x32 },
	{ 159, 162, 0 },
	{ 160, 161, 0 },
	{ 0, 0, 0x30 },
	{ 0, 0, 0x83 },
	{ 0, 0, 0x31 },
	{ 164, 169, 0 },
	{ 165, 168, 0 },
	{ 166, 167, 0 },
	{ 0, 0, 0x2F },
	{ 0, 0, 0x28 },
	{ 0, 0, 0x3D },
	{ 170, 171, 0 },
	{ 0, 0, 0x86 },
	{ 0, 0, 0x5E },
	{ 173, 190, 0 },
	{ 174, 181, 0 },
	{ 175, 178, 0 },
	{ 176, 177, 0 },
	{ 0, 0, 0x33 },
	{ 0, 0, 0x39 },
	{ 179, 180, 0 },
	{ 0, 0, 0x98 },
	{ 0, 0, 0x34 },
	{ 182, 187, 0 },
	{ 183, 186, 0 },
	{ 184, 185, 0 },
	{ 0, 0, 0x7D },
	{ 0, 0, 0x38 },
	{ 0, 0, 0x5C },
	{ 188, 189, 0 },
	{ 0, 0, 0x22 },
	{ 0, 0, 0x90 },
	{ 191, 206, 0 },
	{ 192, 199, 0 },
	{ 193, 196, 0 },
	{ 194, 195, 0 },
	{ 0, 0, 0x26 },
	{ 0, 0, 0x8D },
	{ 197, 198, 0 },
	{ 0, 0, 0x35 },
	{ 0, 0, 0x36 },
	{ 200, 203, 0 },
	{ 201, 202, 0 },
	{ 0, 0, 0x92 },
	{ 0, 0, 0x8F },
	{ 204, 205, 0 },
	{ 0, 0, 0x8E },
	{ 0, 0, 0x93 },
	{ 207, 220, 0 },
	{ 208, 213, 0 },
	{ 209, 212, 0 },
	{ 210, 211, 0 },
	{ 0, 0, 0x8C },
	{ 0, 0, 0x37 },
	{ 0, 0, 0x80 },
	{ 214, 217, 0 },
	{ 215, 216, 0 },
	{ 0, 0, 0x81 },
	{ 0, 0, 0x99 },
	{ 218, 219, 0 },
	{ 0, 0, 0x24 },
	{ 0, 0, 0x40 },
	{ 221, 230, 0 },
	{ 222, 227, 0 },
	{ 223, 226, 0 },
	{ 224, 225, 0 },
	{ 0, 0, 0x5B },
	{ 0, 0, 0x9A },
	{ 0, 0, 0x5F },
	{ 228, 229, 0 },
	{ 0, 0, 0x3E },
	{ 0, 0, 0x96 },
	{ 231, 236, 0 },
	{ 232, 235, 0 },
	{ 233, 234, 0 },
	{ 0, 0, 0x82 },
	{ 0, 0, 0x25 },
	{ 0, 0, 0x09 },
	{ 237, 240, 0 },
	{ 238, 239, 0 },
	{ 0, 0, 0x9C },
	{ 0, 0, 0x97 },
	{ 241, 242, 0 },
	{ 0, 0, 0x7B },
	{ 243, 244, 0 },
	{ 0, 0, 0x94 },
	{ 0, 0, 0x7C },
};

const HuffTree Text::_huffTree_00368[] = {
	{ 1, 20, 0 },
	{ 2, 7, 0 },
	{ 3, 4, 0 },
	{ 0, 0, 0x20 },
	{ 5, 6, 0 },
	{ 0, 0, 0x65 },
	{ 0, 0, 0x61 },
	{ 8, 15, 0 },
	{ 9, 12, 0 },
	{ 10, 11, 0 },
	{ 0, 0, 0x6F },
	{ 0, 0, 0x73 },
	{ 13, 14, 0 },
	{ 0, 0, 0x74 },
	{ 0, 0, 0x6E },
	{ 16, 19, 0 },
	{ 17, 18, 0 },
	{ 0, 0, 0x2E },
	{ 0, 0, 0x69 },
	{ 0, 0, 0x72 },
	{ 21, 44, 0 },
	{ 22, 31, 0 },
	{ 23, 28, 0 },
	{ 24, 27, 0 },
	{ 25, 26, 0 },
	{ 0, 0, 0x00 },
	{ 0, 0, 0x45 },
	{ 0, 0, 0x75 },
	{ 29, 30, 0 },
	{ 0, 0, 0x6D },
	{ 0, 0, 0x41 },
	{ 32, 39, 0 },
	{ 33, 36, 0 },
	{ 34, 35, 0 },
	{ 0, 0, 0x6C },
	{ 0, 0, 0x49 },
	{ 37, 38, 0 },
	{ 0, 0, 0x64 },
	{ 0, 0, 0x52 },
	{ 40, 43, 0 },
	{ 41, 42, 0 },
	{ 0, 0, 0x4E },
	{ 0, 0, 0x53 },
	{ 0, 0, 0x54 },
	{ 45, 68, 0 },
	{ 46, 55, 0 },
	{ 47, 52, 0 },
	{ 48, 51, 0 },
	{ 49, 50, 0 },
	{ 0, 0, 0x4F },
	{ 0, 0, 0x68 },
	{ 0, 0, 0x63 },
	{ 53, 54, 0 },
	{ 0, 0, 0x44 },
	{ 0, 0, 0x67 },
	{ 56, 63, 0 },
	{ 57, 60, 0 },
	{ 58, 59, 0 },
	{ 0, 0, 0x4C },
	{ 0, 0, 0x43 },
	{ 61, 62, 0 },
	{ 0, 0, 0x70 },
	{ 0, 0, 0x55 },
	{ 64, 67, 0 },
	{ 65, 66, 0 },
	{ 0, 0, 0x21 },
	{ 0, 0, 0x79 },
	{ 0, 0, 0x4D },
	{ 69, 88, 0 },
	{ 70, 79, 0 },
	{ 71, 76, 0 },
	{ 72, 75, 0 },
	{ 73, 74, 0 },
	{ 0, 0, 0x50 },
	{ 0, 0, 0x76 },
	{ 0, 0, 0x48 },
	{ 77, 78, 0 },
	{ 0, 0, 0x3F },
	{ 0, 0, 0x62 },
	{ 80, 85, 0 },
	{ 81, 84, 0 },
	{ 82, 83, 0 },
	{ 0, 0, 0x27 },
	{ 0, 0, 0x66 },
	{ 0, 0, 0x2C },
	{ 86, 87, 0 },
	{ 0, 0, 0x47 },
	{ 0, 0, 0x42 },
	{ 89, 108, 0 },
	{ 90, 99, 0 },
	{ 91, 96, 0 },
	{ 92, 95, 0 },
	{ 93, 94, 0 },
	{ 0, 0, 0x56 },
	{ 0, 0, 0x6B },
	{ 0, 0, 0x46 },
	{ 97, 98, 0 },
	{ 0, 0, 0x71 },
	{ 0, 0, 0x77 },
	{ 100, 105, 0 },
	{ 101, 104, 0 },
	{ 102, 103, 0 },
	{ 0, 0, 0x4B },
	{ 0, 0, 0x2D },
	{ 0, 0, 0x57 },
	{ 106, 107, 0 },
	{ 0, 0, 0x4A },
	{ 0, 0, 0x2A },
	{ 109, 128, 0 },
	{ 110, 117, 0 },
	{ 111, 114, 0 },
	{ 112, 113, 0 },
	{ 0, 0, 0x7A },
	{ 0, 0, 0x59 },
	{ 115, 116, 0 },
	{ 0, 0, 0x6A },
	{ 0, 0, 0x2B },
	{ 118, 123, 0 },
	{ 119, 122, 0 },
	{ 120, 121, 0 },
	{ 0, 0, 0x51 },
	{ 0, 0, 0x85 },
	{ 0, 0, 0x29 },
	{ 124, 127, 0 },
	{ 125, 126, 0 },
	{ 0, 0, 0x5A },
	{ 0, 0, 0x8B },
	{ 0, 0, 0x3C },
	{ 129, 152, 0 },
	{ 130, 139, 0 },
	{ 131, 136, 0 },
	{ 132, 135, 0 },
	{ 133, 134, 0 },
	{ 0, 0, 0x95 },
	{ 0, 0, 0x7E },
	{ 0, 0, 0x8A },
	{ 137, 138, 0 },
	{ 0, 0, 0x87 },
	{ 0, 0, 0x3A },
	{ 140, 145, 0 },
	{ 141, 144, 0 },
	{ 142, 143, 0 },
	{ 0, 0, 0x7F },
	{ 0, 0, 0x23 },
	{ 0, 0, 0x78 },
	{ 146, 149, 0 },
	{ 147, 148, 0 },
	{ 0, 0, 0x58 },
	{ 0, 0, 0x5D },
	{ 150, 151, 0 },
	{ 0, 0, 0x91 },
	{ 0, 0, 0x88 },
	{ 153, 174, 0 },
	{ 154, 165, 0 },
	{ 155, 160, 0 },
	{ 156, 159, 0 },
	{ 157, 158, 0 },
	{ 0, 0, 0x5F },
	{ 0, 0, 0x60 },
	{ 0, 0, 0x32 },
	{ 161, 164, 0 },
	{ 162, 163, 0 },
	{ 0, 0, 0x30 },
	{ 0, 0, 0x83 },
	{ 0, 0, 0x31 },
	{ 166, 171, 0 },
	{ 167, 170, 0 },
	{ 168, 169, 0 },
	{ 0, 0, 0x2F },
	{ 0, 0, 0x28 },
	{ 0, 0, 0x3D },
	{ 172, 173, 0 },
	{ 0, 0, 0x86 },
	{ 0, 0, 0x5E },
	{ 175, 190, 0 },
	{ 176, 183, 0 },
	{ 177, 180, 0 },
	{ 178, 179, 0 },
	{ 0, 0, 0x33 },
	{ 0, 0, 0x39 },
	{ 181, 182, 0 },
	{ 0, 0, 0x98 },
	{ 0, 0, 0x34 },
	{ 184, 187, 0 },
	{ 185, 186, 0 },
	{ 0, 0, 0x7D },
	{ 0, 0, 0x38 },
	{ 188, 189, 0 },
	{ 0, 0, 0x5C },
	{ 0, 0, 0x22 },
	{ 191, 206, 0 },
	{ 192, 199, 0 },
	{ 193, 196, 0 },
	{ 194, 195, 0 },
	{ 0, 0, 0x90 },
	{ 0, 0, 0x26 },
	{ 197, 198, 0 },
	{ 0, 0, 0x8D },
	{ 0, 0, 0x35 },
	{ 200, 203, 0 },
	{ 201, 202, 0 },
	{ 0, 0, 0x36 },
	{ 0, 0, 0x3E },
	{ 204, 205, 0 },
	{ 0, 0, 0x93 },
	{ 0, 0, 0x8C },
	{ 207, 220, 0 },
	{ 208, 215, 0 },
	{ 209, 212, 0 },
	{ 210, 211, 0 },
	{ 0, 0, 0x37 },
	{ 0, 0, 0x80 },
	{ 213, 214, 0 },
	{ 0, 0, 0x81 },
	{ 0, 0, 0x8E },
	{ 216, 219, 0 },
	{ 217, 218, 0 },
	{ 0, 0, 0x8F },
	{ 0, 0, 0x99 },
	{ 0, 0, 0x24 },
	{ 221, 230, 0 },
	{ 222, 227, 0 },
	{ 223, 226, 0 },
	{ 224, 225, 0 },
	{ 0, 0, 0x92 },
	{ 0, 0, 0x40 },
	{ 0, 0, 0x5B },
	{ 228, 229, 0 },
	{ 0, 0, 0x9A },
	{ 0, 0, 0x96 },
	{ 231, 236, 0 },
	{ 232, 235, 0 },
	{ 233, 234, 0 },
	{ 0, 0, 0x82 },
	{ 0, 0, 0x25 },
	{ 0, 0, 0x09 },
	{ 237, 240, 0 },
	{ 238, 239, 0 },
	{ 0, 0, 0x9C },
	{ 0, 0, 0x97 },
	{ 241, 242, 0 },
	{ 0, 0, 0x7B },
	{ 243, 244, 0 },
	{ 0, 0, 0x94 },
	{ 0, 0, 0x7C },
};

const HuffTree Text::_huffTree_00372[] = {
	{ 1, 20, 0 },
	{ 2, 7, 0 },
	{ 3, 4, 0 },
	{ 0, 0, ' ' },
	{ 5, 6, 0 },
	{ 0, 0, 'e' },
	{ 0, 0, 'a' },
	{ 8, 15, 0 },
	{ 9, 12, 0 },
	{ 10, 11, 0 },
	{ 0, 0, 'o' },
	{ 0, 0, 's' },
	{ 13, 14, 0 },
	{ 0, 0, 't' },
	{ 0, 0, 'n' },
	{ 16, 19, 0 },
	{ 17, 18, 0 },
	{ 0, 0, '.' },
	{ 0, 0, 'i' },
	{ 0, 0, 'r' },
	{ 21, 44, 0 },
	{ 22, 31, 0 },
	{ 23, 28, 0 },
	{ 24, 27, 0 },
	{ 25, 26, 0 },
	{ 0, 0, 0 },
	{ 0, 0, 'E' },
	{ 0, 0, 'u' },
	{ 29, 30, 0 },
	{ 0, 0, 'm' },
	{ 0, 0, 'A' },
	{ 32, 39, 0 },
	{ 33, 36, 0 },
	{ 34, 35, 0 },
	{ 0, 0, 'l' },
	{ 0, 0, 'I' },
	{ 37, 38, 0 },
	{ 0, 0, 'd' },
	{ 0, 0, 'R' },
	{ 40, 43, 0 },
	{ 41, 42, 0 },
	{ 0, 0, 'N' },
	{ 0, 0, 'S' },
	{ 0, 0, 'T' },
	{ 45, 68, 0 },
	{ 46, 55, 0 },
	{ 47, 52, 0 },
	{ 48, 51, 0 },
	{ 49, 50, 0 },
	{ 0, 0, 'O' },
	{ 0, 0, 'h' },
	{ 0, 0, 'c' },
	{ 53, 54, 0 },
	{ 0, 0, 'D' },
	{ 0, 0, 'g' },
	{ 56, 63, 0 },
	{ 57, 60, 0 },
	{ 58, 59, 0 },
	{ 0, 0, 'L' },
	{ 0, 0, 'C' },
	{ 61, 62, 0 },
	{ 0, 0, 'p' },
	{ 0, 0, 'U' },
	{ 64, 67, 0 },
	{ 65, 66, 0 },
	{ 0, 0, '!' },
	{ 0, 0, 'y' },
	{ 0, 0, 'M' },
	{ 69, 88, 0 },
	{ 70, 79, 0 },
	{ 71, 76, 0 },
	{ 72, 75, 0 },
	{ 73, 74, 0 },
	{ 0, 0, 'P' },
	{ 0, 0, 'v' },
	{ 0, 0, 'H' },
	{ 77, 78, 0 },
	{ 0, 0, '?' },
	{ 0, 0, 'b' },
	{ 80, 85, 0 },
	{ 81, 84, 0 },
	{ 82, 83, 0 },
	{ 0, 0, 39 },
	{ 0, 0, 'f' },
	{ 0, 0, ',' },
	{ 86, 87, 0 },
	{ 0, 0, 'G' },
	{ 0, 0, 'B' },
	{ 89, 108, 0 },
	{ 90, 99, 0 },
	{ 91, 96, 0 },
	{ 92, 95, 0 },
	{ 93, 94, 0 },
	{ 0, 0, 'V' },
	{ 0, 0, 'k' },
	{ 0, 0, 'F' },
	{ 97, 98, 0 },
	{ 0, 0, 'q' },
	{ 0, 0, 'w' },
	{ 100, 105, 0 },
	{ 101, 104, 0 },
	{ 102, 103, 0 },
	{ 0, 0, 'K' },
	{ 0, 0, '-' },
	{ 0, 0, 'W' },
	{ 106, 107, 0 },
	{ 0, 0, 'J' },
	{ 0, 0, '*' },
	{ 109, 128, 0 },
	{ 110, 117, 0 },
	{ 111, 114, 0 },
	{ 112, 113, 0 },
	{ 0, 0, 'z' },
	{ 0, 0, 'Y' },
	{ 115, 116, 0 },
	{ 0, 0, 'j' },
	{ 0, 0, '+' },
	{ 118, 123, 0 },
	{ 119, 122, 0 },
	{ 120, 121, 0 },
	{ 0, 0, 'Q' },
	{ 0, 0, 133 },
	{ 0, 0, ')' },
	{ 124, 127, 0 },
	{ 125, 126, 0 },
	{ 0, 0, 'Z' },
	{ 0, 0, 139 },
	{ 0, 0, '<' },
	{ 129, 150, 0 },
	{ 130, 139, 0 },
	{ 131, 136, 0 },
	{ 132, 135, 0 },
	{ 133, 134, 0 },
	{ 0, 0, 149 },
	{ 0, 0, 126 },
	{ 0, 0, 138 },
	{ 137, 138, 0 },
	{ 0, 0, 135 },
	{ 0, 0, ':' },
	{ 140, 145, 0 },
	{ 141, 144, 0 },
	{ 142, 143, 0 },
	{ 0, 0, 127 },
	{ 0, 0, ']' },
	{ 0, 0, '#' },
	{ 146, 149, 0 },
	{ 147, 148, 0 },
	{ 0, 0, 'x' },
	{ 0, 0, 'X' },
	{ 0, 0, 145 },
	{ 151, 172, 0 },
	{ 152, 163, 0 },
	{ 153, 158, 0 },
	{ 154, 157, 0 },
	{ 155, 156, 0 },
	{ 0, 0, 136 },
	{ 0, 0, '`' },
	{ 0, 0, '2' },
	{ 159, 162, 0 },
	{ 160, 161, 0 },
	{ 0, 0, '0' },
	{ 0, 0, 131 },
	{ 0, 0, '1' },
	{ 164, 169, 0 },
	{ 165, 168, 0 },
	{ 166, 167, 0 },
	{ 0, 0, '/' },
	{ 0, 0, '(' },
	{ 0, 0, '=' },
	{ 170, 171, 0 },
	{ 0, 0, 134 },
	{ 0, 0, '^' },
	{ 173, 190, 0 },
	{ 174, 181, 0 },
	{ 175, 178, 0 },
	{ 176, 177, 0 },
	{ 0, 0, '3' },
	{ 0, 0, '9' },
	{ 179, 180, 0 },
	{ 0, 0, 152 },
	{ 0, 0, '4' },
	{ 182, 187, 0 },
	{ 183, 186, 0 },
	{ 184, 185, 0 },
	{ 0, 0, '}' },
	{ 0, 0, '8' },
	{ 0, 0, '\\' },
	{ 188, 189, 0 },
	{ 0, 0, '"' },
	{ 0, 0, 144 },
	{ 191, 206, 0 },
	{ 192, 199, 0 },
	{ 193, 196, 0 },
	{ 194, 195, 0 },
	{ 0, 0, '&' },
	{ 0, 0, 141 },
	{ 197, 198, 0 },
	{ 0, 0, '5' },
	{ 0, 0, '6' },
	{ 200, 203, 0 },
	{ 201, 202, 0 },
	{ 0, 0, 146 },
	{ 0, 0, 143 },
	{ 204, 205, 0 },
	{ 0, 0, 142 },
	{ 0, 0, 147 },
	{ 207, 220, 0 },
	{ 208, 213, 0 },
	{ 209, 212, 0 },
	{ 210, 211, 0 },
	{ 0, 0, 140 },
	{ 0, 0, '7' },
	{ 0, 0, 128 },
	{ 214, 217, 0 },
	{ 215, 216, 0 },
	{ 0, 0, 129 },
	{ 0, 0, 153 },
	{ 218, 219, 0 },
	{ 0, 0, '$' },
	{ 0, 0, '@' },
	{ 221, 230, 0 },
	{ 222, 227, 0 },
	{ 223, 226, 0 },
	{ 224, 225, 0 },
	{ 0, 0, '[' },
	{ 0, 0, 154 },
	{ 0, 0, '_' },
	{ 228, 229, 0 },
	{ 0, 0, '>' },
	{ 0, 0, 150 },
	{ 231, 236, 0 },
	{ 232, 235, 0 },
	{ 233, 234, 0 },
	{ 0, 0, 130 },
	{ 0, 0, '%' },
	{ 0, 0, 9 },
	{ 237, 240, 0 },
	{ 238, 239, 0 },
	{ 0, 0, 156 },
	{ 0, 0, 151 },
	{ 241, 242, 0 },
	{ 0, 0, '{' },
	{ 243, 244, 0 },
	{ 0, 0, 148 },
	{ 0, 0, '!' },
};
#endif

} // End of namespace Sky

#ifdef PALMOS_68K
#include "scumm_globals.h"

_GINIT(Sky_Hufftext)
_GSETPTR(Sky::Text::_huffTree_00109, GBVARS_HUFFTREE_00109_INDEX, const Sky::HuffTree, GBVARS_QUEEN)
_GSETPTR(Sky::Text::_huffTree_00267, GBVARS_HUFFTREE_00267_INDEX, const Sky::HuffTree, GBVARS_QUEEN)
_GSETPTR(Sky::Text::_huffTree_00288, GBVARS_HUFFTREE_00288_INDEX, const Sky::HuffTree, GBVARS_QUEEN)
_GSETPTR(Sky::Text::_huffTree_00303, GBVARS_HUFFTREE_00303_INDEX, const Sky::HuffTree, GBVARS_QUEEN)
_GSETPTR(Sky::Text::_huffTree_00331, GBVARS_HUFFTREE_00331_INDEX, const Sky::HuffTree, GBVARS_QUEEN)
_GSETPTR(Sky::Text::_huffTree_00348, GBVARS_HUFFTREE_00348_INDEX, const Sky::HuffTree, GBVARS_QUEEN)
_GSETPTR(Sky::Text::_huffTree_00365, GBVARS_HUFFTREE_00365_INDEX, const Sky::HuffTree, GBVARS_QUEEN)
_GSETPTR(Sky::Text::_huffTree_00368, GBVARS_HUFFTREE_00368_INDEX, const Sky::HuffTree, GBVARS_QUEEN)
_GSETPTR(Sky::Text::_huffTree_00372, GBVARS_HUFFTREE_00372_INDEX, const Sky::HuffTree, GBVARS_QUEEN)
_GEND

_GRELEASE(Sky_Hufftext)
_GRELEASEPTR(GBVARS_HUFFTREE_00109_INDEX, GBVARS_QUEEN)
_GRELEASEPTR(GBVARS_HUFFTREE_00267_INDEX, GBVARS_QUEEN)
_GRELEASEPTR(GBVARS_HUFFTREE_00288_INDEX, GBVARS_QUEEN)
_GRELEASEPTR(GBVARS_HUFFTREE_00303_INDEX, GBVARS_QUEEN)
_GRELEASEPTR(GBVARS_HUFFTREE_00331_INDEX, GBVARS_QUEEN)
_GRELEASEPTR(GBVARS_HUFFTREE_00348_INDEX, GBVARS_QUEEN)
_GRELEASEPTR(GBVARS_HUFFTREE_00365_INDEX, GBVARS_QUEEN)
_GRELEASEPTR(GBVARS_HUFFTREE_00368_INDEX, GBVARS_QUEEN)
_GRELEASEPTR(GBVARS_HUFFTREE_00372_INDEX, GBVARS_QUEEN)
_GEND

#endif
