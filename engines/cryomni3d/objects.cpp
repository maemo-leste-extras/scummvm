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


#include "engines/cryomni3d/objects.h"

namespace CryOmni3D {

Object *Objects::findObjectByNameID(unsigned int nameID) {
	for (iterator it = begin(); it != end(); it++) {
		if (it->valid() && it->idOBJ() == nameID) {
			return it;
		}
	}
	error("nameID not found %u", nameID);
}

Object *Objects::findObjectByIconID(unsigned int iconID) {
	for (iterator it = begin(); it != end(); it++) {
		if (it->valid() && it->idCA() == iconID) {
			return it;
		}
	}
	error("iconID not found %u", iconID);
}

void Inventory::clear() {
	for (iterator it = begin(); it != end(); it++) {
		*it = nullptr;
	}
}

void Inventory::add(Object *obj) {
	for (iterator it = begin(); it != end(); it++) {
		if (*it == nullptr) {
			*it = obj;
			(*_changeCallback)(it - begin());
			return;
		}
	}
	error("No more room in inventory");
}

void Inventory::remove(unsigned int position) {
	(*this)[position] = nullptr;
	(*_changeCallback)(-1u);
}

void Inventory::removeByIconID(unsigned int iconID) {
	for (iterator it = begin(); it != end(); it++) {
		if ((*it) && (*it)->idCA() == iconID) {
			deselectObject();
			remove(it - begin());
			return;
		}
	}
	// Don't bail out
}

void Inventory::removeByNameID(unsigned int nameID) {
	for (iterator it = begin(); it != end(); it++) {
		if ((*it) && (*it)->idOBJ() == nameID) {
			deselectObject();
			remove(it - begin());
			return;
		}
	}
	// Don't bail out
}

bool Inventory::inInventoryByIconID(unsigned int iconID) const {
	for (const_iterator it = begin(); it != end(); it++) {
		if ((*it) && (*it)->idCA() == iconID) {
			return true;
		}
	}
	return false;
}

bool Inventory::inInventoryByNameID(unsigned int nameID) const {
	for (const_iterator it = begin(); it != end(); it++) {
		if ((*it) && (*it)->idOBJ() == nameID) {
			return true;
		}
	}
	return false;
}

} // End of namespace CryOmni3D
