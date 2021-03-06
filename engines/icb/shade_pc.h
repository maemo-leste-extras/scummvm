/* ResidualVM - A 3D game interpreter
 *
 * ResidualVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the AUTHORS
 * file distributed with this source distribution.
 *
 * Additional copyright for this file:
 * Copyright (C) 1999-2000 Revolution Software Ltd.
 * This code is based on source code created by Revolution Software,
 * used with permission.
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

#ifndef ICB_SHADE_PC_H
#define ICB_SHADE_PC_H

#include "engines/icb/gfx/psx_pcdefines.h"
#include "engines/icb/gfx/rlp_api.h"

namespace ICB {

int32 computeShadeMultiplierPC(ShadeTriangle *shadeTri, VECTOR *model_pos, VECTOR *light_pos, int32 m);

int32 computeShadeMultiplierPC(ShadeQuad *shadeQuad, VECTOR *model_pos, VECTOR *light_pos, int32 m);

} // End of namespace ICB

#endif // #ifndef SHADE_PC_H
