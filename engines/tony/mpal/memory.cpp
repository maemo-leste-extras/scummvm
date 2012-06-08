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

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 *
 */

#include "common/algorithm.h"
#include "common/textconsole.h"
#include "tony/mpal/memory.h"

namespace Tony {

namespace MPAL {

/****************************************************************************\
*       MemoryManager methods
\****************************************************************************/

const uint32 BLOCK_ID = 0x12345678;

/**
 * Allocates a new memory block
 * @return					Returns a MemoryItem instance for the new block
 */
HANDLE MemoryManager::allocate(uint32 size, uint flags) {
	MemoryItem *newItem = (MemoryItem *)malloc(sizeof(MemoryItem) + size);
	newItem->_id = BLOCK_ID;
	newItem->_size = size;
	newItem->_lockCount = 0;

	// If requested, clear the allocated data block
	if ((flags & GMEM_ZEROINIT) != 0) {
		byte *dataP = newItem->_data;
		Common::fill(dataP, dataP + size, 0);
	}

	return (HANDLE)newItem;
}

/**
 * Allocates a new memory block and returns its data pointer
 * @return					Data pointer to allocated block
 */
void *MemoryManager::alloc(uint32 size, uint flags) {
	MemoryItem *item = (MemoryItem *)allocate(size, flags);
	++item->_lockCount;
	return &item->_data[0];
}

#define OFFSETOF(type, field)    ((unsigned long) &(((type *) 0)->field))

/**
 * Returns a reference to the MemoryItem for a gien byte pointer
 * @param block				Byte pointer
 */
MemoryItem *MemoryManager::getItem(HGLOBAL handle) {
	MemoryItem *rec = (MemoryItem *)((byte *)handle - OFFSETOF(MemoryItem, _data));
	assert(rec->_id == BLOCK_ID);
	return rec;
}

/**
 * Returns a size of a memory block given its pointer
 */
uint32 MemoryManager::getSize(HANDLE handle) {
	MemoryItem *item = (MemoryItem *)handle;
	assert(item->_id == BLOCK_ID);
	return item->_size;
}

/**
 * Erases a given item
 */
void MemoryManager::freeBlock(HANDLE handle) {
	MemoryItem *item = (MemoryItem *)handle;
	assert(item->_id == BLOCK_ID);
	free(item);
}

/**
 * Erases a given item
 */
void MemoryManager::destroyItem(HANDLE handle) {
	MemoryItem *item = getItem(handle);
	assert(item->_id == BLOCK_ID);
	free(item);
}

/**
 * Locks an item for access
 */
byte *MemoryManager::lockItem(HANDLE handle) {
	MemoryItem *item = (MemoryItem *)handle;
	assert(item->_id == BLOCK_ID);
	++item->_lockCount;
	return &item->_data[0];
}

/**
 * Unlocks a locked item
 */
void MemoryManager::unlockItem(HANDLE handle) {
	MemoryItem *item = (MemoryItem *)handle;
	assert(item->_id == BLOCK_ID);
	assert(item->_lockCount > 0);
	--item->_lockCount;
}


/****************************************************************************\
*       Stand-alone methods
\****************************************************************************/

void copyMemory(void *dst, const void *first, int size) {
	Common::copy((const byte *)first, (const byte *)first + size, (byte *)dst);
}


} // end of namespace MPAL

} // end of namespace Tony
