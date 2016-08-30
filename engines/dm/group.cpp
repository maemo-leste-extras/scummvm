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

/*
* Based on the Reverse Engineering work of Christophe Fontanel,
* maintainer of the Dungeon Master Encyclopaedia (http://dmweb.free.fr/)
*/

#include "group.h"
#include "dungeonman.h"
#include "champion.h"
#include "movesens.h"
#include "projexpl.h"
#include "timeline.h"
#include "objectman.h"
#include "menus.h"
#include "sounds.h"


namespace DM {
int32 M32_setTime(int32 &map_time, int32 time) {
	return map_time = (map_time & 0xFF000000) | time;
}

GroupMan::GroupMan(DMEngine *vm) : _vm(vm) {
	for (uint16 i = 0; i < 4; ++i)
		_dropMovingCreatureFixedPossessionsCell[i] = 0;
	_dropMovingCreatureFixedPossCellCount = 0;
	_fluxCageCount = 0;
	for (uint16 i = 0; i < 4; ++i)
		_fluxCages[i] = 0;
	_currentGroupMapX = 0;
	_currentGroupMapY = 0;
	_currGroupThing = Thing(0);
	for (uint16 i = 0; i < 4; ++i)
		_groupMovementTestedDirections[i] = 0;
	_currGroupDistanceToParty = 0;
	_currGroupPrimaryDirToParty = 0;
	_currGroupSecondaryDirToParty = 0;
	_groupMovementBlockedByGroupThing = Thing(0);
	_groupMovementBlockedByDoor = false;
	_groupMovementBlockedByParty = false;
	_groupMovBlockedByWallStairsPitFakeWalFluxCageTeleporter = false;
	_maxActiveGroupCount = 60;
	_activeGroups = nullptr;
	_currActiveGroupCount = 0;
	twoHalfSquareSizedCreaturesGroupLastDirectionSetTime = 0;
}

GroupMan::~GroupMan() {
	delete[] _activeGroups;
}

void GroupMan::initActiveGroups() {
	if (_vm->_newGameFl)
		_maxActiveGroupCount = 60;

	if (_activeGroups)
		delete[] _activeGroups;

	_activeGroups = new ActiveGroup[_maxActiveGroupCount];
	for (uint16 i = 0; i < _maxActiveGroupCount; ++i)
		_activeGroups[i]._groupThingIndex = -1;
}

uint16 GroupMan::getGroupCells(Group *group, int16 mapIndex) {
	byte cells = group->_cells;
	if (mapIndex == _vm->_dungeonMan->_partyMapIndex)
		cells = _activeGroups[cells]._cells;
	return cells;
}

uint16 GroupMan::getGroupDirections(Group *group, int16 mapIndex) {
	static byte groupDirections[4] = {0x00, 0x55, 0xAA, 0xFF}; // @ G0258_auc_Graphic559_GroupDirections

	if (mapIndex == _vm->_dungeonMan->_partyMapIndex)
		return _activeGroups[group->getActiveGroupIndex()]._directions;

	return groupDirections[group->getDir()];
}

int16 GroupMan::getCreatureOrdinalInCell(Group *group, uint16 cell) {
	uint16 currMapIndex = _vm->_dungeonMan->_currMapIndex;
	byte groupCells = getGroupCells(group, currMapIndex);
	if (groupCells == k255_CreatureTypeSingleCenteredCreature)
		return _vm->indexToOrdinal(0);

	int retval = 0;
	byte creatureIndex = group->getCount();
	if (getFlag(_vm->_dungeonMan->_creatureInfos[group->_type]._attributes, k0x0003_MaskCreatureInfo_size) == k1_MaskCreatureSizeHalf) {
		if ((getGroupDirections(group, currMapIndex) & 1) == (cell & 1))
			cell = returnPrevVal(cell);

		do {
			byte creatureCell = getCreatureValue(groupCells, creatureIndex);
			if (creatureCell == cell || creatureCell == returnNextVal(cell)) {
				retval = _vm->indexToOrdinal(creatureIndex);
				break;
			}
		} while (creatureIndex--);
	} else {
		do {
			if (getCreatureValue(groupCells, creatureIndex) == cell) {
				retval = _vm->indexToOrdinal(creatureIndex);
				break;
			}
		} while (creatureIndex--);
	}

	return retval;
}

uint16 GroupMan::getCreatureValue(uint16 groupVal, uint16 creatureIndex) {
	return (groupVal >> (creatureIndex << 1)) & 0x3;
}

void GroupMan::dropGroupPossessions(int16 mapX, int16 mapY, Thing groupThing, int16 mode) {
	Group *group = (Group *)_vm->_dungeonMan->getThingData(groupThing);
	uint16 creatureType = group->_type;
	if ((mode >= k0_soundModePlayImmediately) && getFlag(_vm->_dungeonMan->_creatureInfos[creatureType]._attributes, k0x0200_MaskCreatureInfo_dropFixedPoss)) {
		int16 creatureIndex = group->getCount();
		uint16 groupCells = getGroupCells(group, _vm->_dungeonMan->_currMapIndex);
		do {
			dropCreatureFixedPossessions(creatureType, mapX, mapY,
				(groupCells == k255_CreatureTypeSingleCenteredCreature) ? k255_CreatureTypeSingleCenteredCreature : getCreatureValue(groupCells, creatureIndex), mode);
		} while (creatureIndex--);
	}

	Thing currentThing = group->_slot;
	if ((currentThing) != Thing::_endOfList) {
		bool L0371_B_WeaponDropped = false;
		Thing nextThing;
		do {
			nextThing = _vm->_dungeonMan->getNextThing(currentThing);
			currentThing = thingWithNewCell(currentThing, _vm->getRandomNumber(4));
			if ((currentThing).getType() == k5_WeaponThingType) {
				L0371_B_WeaponDropped = true;
			}
			_vm->_moveSens->getMoveResult(currentThing, kM1_MapXNotOnASquare, 0, mapX, mapY);
		} while ((currentThing = nextThing) != Thing::_endOfList);

		if (mode >= k0_soundModePlayImmediately)
			_vm->_sound->requestPlay(L0371_B_WeaponDropped ? k00_soundMETALLIC_THUD : k04_soundWOODEN_THUD_ATTACK_TROLIN_ANTMAN_STONE_GOLEM, mapX, mapY, mode);
	}
}

void GroupMan::dropCreatureFixedPossessions(uint16 creatureType, int16 mapX, int16 mapY, uint16 cell, int16 mode) {
	static uint16 fixedPossessionCreature12Skeleton[3] = { // @ G0245_aui_Graphic559_FixedPossessionsCreature12Skeleton
		k23_ObjectInfoIndexFirstWeapon + k9_WeaponTypeFalchion,
		k69_ObjectInfoIndexFirstArmour + k30_ArmourTypeWoodenShield,
		0}
	;
	static uint16 fixedPossessionCreature9StoneGolem[2] = { // @ G0246_aui_Graphic559_FixedPossessionsCreature09StoneGolem
		k23_ObjectInfoIndexFirstWeapon + k24_WeaponTypeStoneClub,
		0
	};
	static uint16 fixedPossessionCreatur16TrolinAntman[2] = { // @ G0247_aui_Graphic559_FixedPossessionsCreature16Trolin_Antman
		k23_ObjectInfoIndexFirstWeapon + k23_WeaponTypeClub,
		0
	};
	static uint16 fixedPossessionCreature18AnimatedArmourDethKnight[7] = { // @ G0248_aui_Graphic559_FixedPossessionsCreature18AnimatedArmour_DethKnight
		k69_ObjectInfoIndexFirstArmour + k41_ArmourTypeFootPlate,
		k69_ObjectInfoIndexFirstArmour + k40_ArmourTypeLegPlate,
		k69_ObjectInfoIndexFirstArmour + k39_ArmourTypeTorsoPlate,
		k23_ObjectInfoIndexFirstWeapon + k10_WeaponTypeSword,
		k69_ObjectInfoIndexFirstArmour + k38_ArmourTypeArmet,
		k23_ObjectInfoIndexFirstWeapon + k10_WeaponTypeSword,
		0
	};
	static uint16 fixedPossessionCreature7rockRockPile[5] = { // @ G0249_aui_Graphic559_FixedPossessionsCreature07Rock_RockPile
		k127_ObjectInfoIndexFirstJunk + k25_JunkTypeBoulder,
		k127_ObjectInfoIndexFirstJunk + k25_JunkTypeBoulder | k0x8000_randomDrop,
		k23_ObjectInfoIndexFirstWeapon + k30_WeaponTypeRock | k0x8000_randomDrop,
		k23_ObjectInfoIndexFirstWeapon + k30_WeaponTypeRock | k0x8000_randomDrop,
		0
	};
	static uint16 fixedPossessionCreature4PainRatHellHound[3] = { // @ G0250_aui_Graphic559_FixedPossessionsCreature04PainRat_Hellhound
		k127_ObjectInfoIndexFirstJunk + k35_JunkTypeDrumstickShank,
		k127_ObjectInfoIndexFirstJunk + k35_JunkTypeDrumstickShank | k0x8000_randomDrop,
		0
	};
	static uint16 fixedPossessionCreature6screamer[3] = { // @ G0251_aui_Graphic559_FixedPossessionsCreature06Screamer
		k127_ObjectInfoIndexFirstJunk + k33_JunkTypeScreamerSlice,
		k127_ObjectInfoIndexFirstJunk + k33_JunkTypeScreamerSlice | k0x8000_randomDrop,
		0
	};
	static uint16 fixedPossessionCreature15MagnetaWormWorm[4] = { // @ G0252_aui_Graphic559_FixedPossessionsCreature15MagentaWorm_Worm
		k127_ObjectInfoIndexFirstJunk + k34_JunkTypeWormRound,
		k127_ObjectInfoIndexFirstJunk + k34_JunkTypeWormRound | k0x8000_randomDrop,
		k127_ObjectInfoIndexFirstJunk + k34_JunkTypeWormRound | k0x8000_randomDrop,
		0
	};
	static uint16 fixedPossessionCreature24RedDragon[11] = { // @ G0253_aui_Graphic559_FixedPossessionsCreature24RedDragon
		k127_ObjectInfoIndexFirstJunk + k36_JunkTypeDragonSteak,
		k127_ObjectInfoIndexFirstJunk + k36_JunkTypeDragonSteak,
		k127_ObjectInfoIndexFirstJunk + k36_JunkTypeDragonSteak,
		k127_ObjectInfoIndexFirstJunk + k36_JunkTypeDragonSteak,
		k127_ObjectInfoIndexFirstJunk + k36_JunkTypeDragonSteak,
		k127_ObjectInfoIndexFirstJunk + k36_JunkTypeDragonSteak,
		k127_ObjectInfoIndexFirstJunk + k36_JunkTypeDragonSteak,
		k127_ObjectInfoIndexFirstJunk + k36_JunkTypeDragonSteak,
		k127_ObjectInfoIndexFirstJunk + k36_JunkTypeDragonSteak | k0x8000_randomDrop,
		k127_ObjectInfoIndexFirstJunk + k36_JunkTypeDragonSteak | k0x8000_randomDrop, 0};

	uint16 *fixedPossessions;
	bool cursedPossessions = false;
	switch (creatureType) {
	case k4_CreatureTypePainRatHellHound:
		fixedPossessions = fixedPossessionCreature4PainRatHellHound;
		break;
	case k6_CreatureTypeScreamer:
		fixedPossessions = fixedPossessionCreature6screamer;
		break;
	case k7_CreatureTypeRockpile:
		fixedPossessions = fixedPossessionCreature7rockRockPile;
		break;
	case k9_CreatureTypeStoneGolem:
		fixedPossessions = fixedPossessionCreature9StoneGolem;
		break;
	case k12_CreatureTypeSkeleton:
		fixedPossessions = fixedPossessionCreature12Skeleton;
		break;
	case k16_CreatureTypeTrolinAntman:
		fixedPossessions = fixedPossessionCreatur16TrolinAntman;
		break;
	case k15_CreatureTypeMagnetaWormWorm:
		fixedPossessions = fixedPossessionCreature15MagnetaWormWorm;
		break;
	case k18_CreatureTypeAnimatedArmourDethKnight:
		cursedPossessions = true;
		fixedPossessions = fixedPossessionCreature18AnimatedArmourDethKnight;
		break;
	case k24_CreatureTypeRedDragon:
		fixedPossessions = fixedPossessionCreature24RedDragon;
		break;
	default:
		return;
	}

	uint16 currFixedPossession = *fixedPossessions++;
	bool weaponDropped = false;
	while (currFixedPossession) {
		if (getFlag(currFixedPossession, k0x8000_randomDrop) && _vm->getRandomNumber(2))
			continue;

		int16 currThingType;
		if (clearFlag(currFixedPossession, k0x8000_randomDrop) >= k127_ObjectInfoIndexFirstJunk) {
			currThingType = k10_JunkThingType;
			currFixedPossession -= k127_ObjectInfoIndexFirstJunk;
		} else if (currFixedPossession >= k69_ObjectInfoIndexFirstArmour) {
			currThingType = k6_ArmourThingType;
			currFixedPossession -= k69_ObjectInfoIndexFirstArmour;
		} else {
			weaponDropped = true;
			currThingType = k5_WeaponThingType;
			currFixedPossession -= k23_ObjectInfoIndexFirstWeapon;
		}

		Thing nextUnusedThing = _vm->_dungeonMan->getUnusedThing(currThingType);
		if ((nextUnusedThing) == Thing::_none)
			continue;

		Weapon *currWeapon = (Weapon *)_vm->_dungeonMan->getThingData(nextUnusedThing);
/* The same pointer type is used no matter the actual type k5_WeaponThingType, k6_ArmourThingType or k10_JunkThingType */
		currWeapon->setType(currFixedPossession);
		currWeapon->setCursed(cursedPossessions);
		nextUnusedThing = thingWithNewCell(nextUnusedThing, ((cell == k255_CreatureTypeSingleCenteredCreature) || !_vm->getRandomNumber(4)) ? _vm->getRandomNumber(4) : cell);
		_vm->_moveSens->getMoveResult(nextUnusedThing, kM1_MapXNotOnASquare, 0, mapX, mapY);
		currFixedPossession = *fixedPossessions++;
	}
	_vm->_sound->requestPlay(weaponDropped ? k00_soundMETALLIC_THUD : k04_soundWOODEN_THUD_ATTACK_TROLIN_ANTMAN_STONE_GOLEM, mapX, mapY, mode);
}

int16 GroupMan::getDirsWhereDestIsVisibleFromSource(int16 srcMapX, int16 srcMapY, int16 destMapX, int16 destMapY) {
	if (srcMapX == destMapX) {
		_vm->_projexpl->_secondaryDirToOrFromParty = (_vm->getRandomNumber(65536) & 0x0002) + 1; /* Resulting direction may be 1 or 3 (East or West) */
		if (srcMapY > destMapY)
			return kDirNorth;

		return kDirSouth;
	}
	if (srcMapY == destMapY) {
		_vm->_projexpl->_secondaryDirToOrFromParty = (_vm->getRandomNumber(65536) & 0x0002) + 0; /* Resulting direction may be 0 or 2 (North or South) */
		if (srcMapX > destMapX)
			return kDirWest;

		return kDirEast;
	}

	int16 curDirection = kDirNorth;
	for (;;) {
		if (isDestVisibleFromSource(curDirection, srcMapX, srcMapY, destMapX, destMapY)) {
			_vm->_projexpl->_secondaryDirToOrFromParty = returnNextVal(curDirection);
			if (!isDestVisibleFromSource(_vm->_projexpl->_secondaryDirToOrFromParty, srcMapX, srcMapY, destMapX, destMapY)) {
				_vm->_projexpl->_secondaryDirToOrFromParty = returnPrevVal(curDirection);
				if ((curDirection != kDirNorth) || !isDestVisibleFromSource(_vm->_projexpl->_secondaryDirToOrFromParty, srcMapX, srcMapY, destMapX, destMapY)) {
					_vm->_projexpl->_secondaryDirToOrFromParty = returnNextVal((_vm->getRandomNumber(65536) & 0x0002) + curDirection);
					return curDirection;
				}
			}
			if (_vm->getRandomNumber(2)) {
				int16 primaryDirection = _vm->_projexpl->_secondaryDirToOrFromParty;
				_vm->_projexpl->_secondaryDirToOrFromParty = curDirection;
				return primaryDirection;
			}
			return curDirection;
		}
		curDirection++;
	}
}

bool GroupMan::isDestVisibleFromSource(uint16 dir, int16 srcMapX, int16 srcMapY, int16 destMapX, int16 destMapY) {
	switch (dir) { /* If direction is not 'West' then swap variables so that the same test as for west can be applied */
	case kDirSouth:
		SWAP(srcMapX, destMapY);
		SWAP(destMapX, srcMapY);
		break;
	case kDirEast:
		SWAP(srcMapX, destMapX);
		SWAP(destMapY, srcMapY);
		break;
	case kDirNorth:
		SWAP(srcMapX, srcMapY);
		SWAP(destMapX, destMapY);
		break;
	}
	return ((srcMapX -= (destMapX - 1)) > 0) && ((((srcMapY -= destMapY) < 0) ? -srcMapY : srcMapY) <= srcMapX);
}

bool GroupMan::groupIsDoorDestoryedByAttack(uint16 mapX, uint16 mapY, int16 attack, bool magicAttack, int16 ticks) {
	Door *curDoor = (Door *)_vm->_dungeonMan->getSquareFirstThingData(mapX, mapY);
	if ((magicAttack && !curDoor->isMagicDestructible()) || (!magicAttack && !curDoor->isMeleeDestructible()))
		return false;

	if (attack >= _vm->_dungeonMan->_currMapDoorInfo[curDoor->getType()]._defense) {
		byte *curSquare = &_vm->_dungeonMan->_currMapData[mapX][mapY];
		if (Square(*curSquare).getDoorState() == k4_doorState_CLOSED) {
			if (ticks) {
				TimelineEvent newEvent;
				setMapAndTime(newEvent._mapTime, _vm->_dungeonMan->_currMapIndex, _vm->_gameTime + ticks);
				newEvent._type = k2_TMEventTypeDoorDestruction;
				newEvent._priority = 0;
				newEvent._B._location._mapX = mapX;
				newEvent._B._location._mapY = mapY;
				_vm->_timeline->addEventGetEventIndex(&newEvent);
			} else {
				((Square *)curSquare)->setDoorState(k5_doorState_DESTROYED);
			}
			return true;
		}
	}
	return false;
}

Thing GroupMan::groupGetThing(int16 mapX, int16 mapY) {
	Thing curThing = _vm->_dungeonMan->getSquareFirstThing(mapX, mapY);
	while ((curThing != Thing::_endOfList) && (curThing.getType() != k4_GroupThingType))
		curThing = _vm->_dungeonMan->getNextThing(curThing);

	return curThing;
}

int16 GroupMan::groupGetDamageCreatureOutcome(Group *group, uint16 creatureIndex, int16 mapX, int16 mapY, int16 damage, bool notMoving) {
	uint16 creatureType = group->_type;
	CreatureInfo *creatureInfo = &_vm->_dungeonMan->_creatureInfos[creatureType];
	if (getFlag(creatureInfo->_attributes, k0x2000_MaskCreatureInfo_archenemy)) /* Lord Chaos cannot be damaged */
		return k0_outcomeKilledNoCreaturesInGroup;

	if (group->_health[creatureIndex] <= damage) {
		uint16 groupCells = getGroupCells(group, _vm->_dungeonMan->_currMapIndex);
		uint16 cell = (groupCells == k255_CreatureTypeSingleCenteredCreature) ? k255_CreatureTypeSingleCenteredCreature : getCreatureValue(groupCells, creatureIndex);
		uint16 creatureCount = group->getCount();
		uint16 retVal;

		if (!creatureCount) { /* If there is a single creature in the group */
			if (notMoving) {
				dropGroupPossessions(mapX, mapY, groupGetThing(mapX, mapY), k2_soundModePlayOneTickLater);
				groupDelete(mapX, mapY);
			}
			retVal = k2_outcomeKilledAllCreaturesInGroup;
		} else { /* If there are several creatures in the group */
			uint16 groupDirections = getGroupDirections(group, _vm->_dungeonMan->_currMapIndex);
			if (getFlag(creatureInfo->_attributes, k0x0200_MaskCreatureInfo_dropFixedPoss)) {
				if (notMoving)
					dropCreatureFixedPossessions(creatureType, mapX, mapY, cell, k2_soundModePlayOneTickLater);
				else
					_dropMovingCreatureFixedPossessionsCell[_dropMovingCreatureFixedPossCellCount++] = cell;
			}
			bool currentMapIsPartyMap = (_vm->_dungeonMan->_currMapIndex == _vm->_dungeonMan->_partyMapIndex);
			ActiveGroup *activeGroup = nullptr;
			if (currentMapIsPartyMap)
				activeGroup = &_activeGroups[group->getActiveGroupIndex()];

			if (group->getBehaviour() == k6_behavior_ATTACK) {
				TimelineEvent *curEvent = _vm->_timeline->_events;
				for (uint16 eventIndex = 0; eventIndex < _vm->_timeline->_eventMaxCount; eventIndex++) {
					uint16 curEventType = curEvent->_type;
					if ((getMap(curEvent->_mapTime) == _vm->_dungeonMan->_currMapIndex) &&
						(curEvent->_B._location._mapX == mapX) &&
						(curEvent->_B._location._mapY == mapY) &&
						(curEventType > k32_TMEventTypeUpdateAspectGroup) &&
						(curEventType < k41_TMEventTypeUpdateBehaviour_3 + 1)) {
						uint16 nextCreatureIndex;
						if (curEventType < k37_TMEventTypeUpdateBehaviourGroup)
							nextCreatureIndex = curEventType - k33_TMEventTypeUpdateAspectCreature_0; /* Get creature index for events 33 to 36 */
						else
							nextCreatureIndex = curEventType - k38_TMEventTypeUpdateBehaviour_0; /* Get creature index for events 38 to 41 */

						if (nextCreatureIndex == creatureIndex)
							_vm->_timeline->deleteEvent(eventIndex);
						else if (nextCreatureIndex > creatureIndex) {
							curEvent->_type -= 1;
							_vm->_timeline->fixChronology(_vm->_timeline->getIndex(eventIndex));
						}
					}
					curEvent++;
				}

				uint16 fearResistance = creatureInfo->getFearResistance();
				if (currentMapIsPartyMap && (fearResistance) != k15_immuneToFear) {
					fearResistance += creatureCount - 1;
					if (fearResistance < _vm->getRandomNumber(16)) { /* Test if the death of a creature frightens the remaining creatures in the group */
						activeGroup->_delayFleeingFromTarget = _vm->getRandomNumber(100 - (fearResistance << 2)) + 20;
						group->setBehaviour(k5_behavior_FLEE);
					}
				}
			}
			uint16 nextCreatureIndex = creatureIndex;
			for (uint16 curCreatureIndex = creatureIndex; curCreatureIndex < creatureCount; curCreatureIndex++) {
				nextCreatureIndex++;
				group->_health[curCreatureIndex] = group->_health[nextCreatureIndex];
				groupDirections = getGroupValueUpdatedWithCreatureValue(groupDirections, curCreatureIndex, getCreatureValue(groupDirections, nextCreatureIndex));
				groupCells = getGroupValueUpdatedWithCreatureValue(groupCells, curCreatureIndex, getCreatureValue(groupCells, nextCreatureIndex));
				if (currentMapIsPartyMap)
					activeGroup->_aspect[curCreatureIndex] = activeGroup->_aspect[nextCreatureIndex];
			}
			groupCells &= 0x003F;
			_vm->_dungeonMan->setGroupCells(group, groupCells, _vm->_dungeonMan->_currMapIndex);
			_vm->_dungeonMan->setGroupDirections(group, groupDirections, _vm->_dungeonMan->_currMapIndex);
			group->setCount(group->getCount() - 1);
			retVal = k1_outcomeKilledSomeCreaturesInGroup;
		}

		uint16 creatureSize = getFlag(creatureInfo->_attributes, k0x0003_MaskCreatureInfo_size);
		uint16 attack;
		if (creatureSize == k0_MaskCreatureSizeQuarter)
			attack = 110;
		else if (creatureSize == k1_MaskCreatureSizeHalf)
			attack = 190;
		else
			attack = 255;

		_vm->_projexpl->createExplosion(Thing::_explSmoke, attack, mapX, mapY, cell); /* BUG0_66 Smoke is placed on the source map instead of the destination map when a creature dies by falling through a pit. The game has a special case to correctly drop the creature possessions on the destination map but there is no such special case for the smoke. Note that the death must be caused by the damage of the fall (there is no smoke if the creature is removed because its type is not allowed on the destination map). However this bug has no visible consequence because of BUG0_26: the smoke explosion falls in the pit right after being placed in the dungeon and before being drawn on screen so it is only visible on the destination square */
		return retVal;
	}

	if (damage > 0)
		group->_health[creatureIndex] -= damage;

	return k0_outcomeKilledNoCreaturesInGroup;
}

void GroupMan::groupDelete(int16 mapX, int16 mapY) {
	Thing groupThing = groupGetThing(mapX, mapY);
	if (groupThing == Thing::_endOfList)
		return;

	Group *group = (Group *)_vm->_dungeonMan->getThingData(groupThing);
	for (uint16 i = 0; i < 4; ++i)
		group->_health[i] = 0;
	_vm->_moveSens->getMoveResult(groupThing, mapX, mapY, kM1_MapXNotOnASquare, 0);
	group->_nextThing = Thing::_none;
	if (_vm->_dungeonMan->_currMapIndex == _vm->_dungeonMan->_partyMapIndex) {
		_activeGroups[group->getActiveGroupIndex()]._groupThingIndex = -1;
		_currActiveGroupCount--;
	}
	groupDeleteEvents(mapX, mapY);
}

void GroupMan::groupDeleteEvents(int16 mapX, int16 mapY) {
	TimelineEvent *curEvent = _vm->_timeline->_events;
	for (int16 eventIndex = 0; eventIndex < _vm->_timeline->_eventMaxCount; eventIndex++) {
		uint16 curEventType = curEvent->_type;
		if ((getMap(curEvent->_mapTime) == _vm->_dungeonMan->_currMapIndex) &&
			(curEventType > k29_TMEventTypeGroupReactionDangerOnSquare - 1) && (curEventType < k41_TMEventTypeUpdateBehaviour_3 + 1) &&
			(curEvent->_B._location._mapX == mapX) && (curEvent->_B._location._mapY == mapY)) {
			_vm->_timeline->deleteEvent(eventIndex);
		}
		curEvent++;
	}
}

uint16 GroupMan::getGroupValueUpdatedWithCreatureValue(uint16 groupVal, uint16 creatureIndex, uint16 creatureVal) {
	creatureVal &= 0x0003;
	creatureIndex <<= 1;
	creatureVal <<= creatureIndex;
	return creatureVal | (groupVal & ~(3 << creatureVal));
}

int16 GroupMan::getDamageAllCreaturesOutcome(Group *group, int16 mapX, int16 mapY, int16 attack, bool notMoving) {
	bool killedSomeCreatures = false;
	bool killedAllCreatures = true;
	_dropMovingCreatureFixedPossCellCount = 0;
	if (attack > 0) {
		int16 creatureIndex = group->getCount();
		uint16 randomAttackSeed = (attack >> 3) + 1;
		attack -= randomAttackSeed;
		randomAttackSeed <<= 1;
		do {
			int16 outcomeVal = groupGetDamageCreatureOutcome(group, creatureIndex, mapX, mapY, attack + _vm->getRandomNumber(randomAttackSeed), notMoving);
			killedAllCreatures = outcomeVal && killedAllCreatures;
			killedSomeCreatures = killedSomeCreatures || outcomeVal;
		} while (creatureIndex--);
		if (killedAllCreatures)
			return k2_outcomeKilledAllCreaturesInGroup;

		if (killedSomeCreatures)
			return k1_outcomeKilledSomeCreaturesInGroup;
	}
	
	return k0_outcomeKilledNoCreaturesInGroup;
}

int16 GroupMan::groupGetResistanceAdjustedPoisonAttack(uint16 creatreType, int16 poisonAttack) {
	int16 poisonResistance = _vm->_dungeonMan->_creatureInfos[creatreType].getPoisonResistance();

	if (!poisonAttack || (poisonResistance == k15_immuneToPoison))
		return 0;

	return ((poisonAttack + _vm->getRandomNumber(4)) << 3) / (poisonResistance + 1);
}

void GroupMan::processEvents29to41(int16 eventMapX, int16 eventMapY, int16 eventType, uint16 ticks) {
	int16 L0446_i_Multiple = 0;
#define AL0446_i_Direction           L0446_i_Multiple
#define AL0446_i_Ticks               L0446_i_Multiple
#define AL0446_i_Behavior2Or3        L0446_i_Multiple
#define AL0446_i_CreatureAspectIndex L0446_i_Multiple
#define AL0446_i_Range               L0446_i_Multiple
#define AL0446_i_CreatureAttributes  L0446_i_Multiple
#define AL0446_i_Cell                L0446_i_Multiple
#define AL0446_i_GroupCellsCriteria  L0446_i_Multiple
	int16 L0447_i_Multiple;
#define AL0447_i_Behavior           L0447_i_Multiple
#define AL0447_i_CreatureIndex      L0447_i_Multiple
#define AL0447_i_ReferenceDirection L0447_i_Multiple
#define AL0447_i_Ticks              L0447_i_Multiple
	int16 L0450_i_Multiple;
#define AL0450_i_DestinationMapX  L0450_i_Multiple
#define AL0450_i_DistanceXToParty L0450_i_Multiple
#define AL0450_i_TargetMapX       L0450_i_Multiple
	int16 L0451_i_Multiple;
#define AL0451_i_DestinationMapY  L0451_i_Multiple
#define AL0451_i_DistanceYToParty L0451_i_Multiple
#define AL0451_i_TargetMapY       L0451_i_Multiple

	/* If the party is not on the map specified in the event and the event type is not one of 32, 33, 37, 38 then the event is ignored */
	if ((_vm->_dungeonMan->_currMapIndex != _vm->_dungeonMan->_partyMapIndex)
	 && (eventType != k37_TMEventTypeUpdateBehaviourGroup) && (eventType != k32_TMEventTypeUpdateAspectGroup)
	 && (eventType != k38_TMEventTypeUpdateBehaviour_0) && (eventType != k33_TMEventTypeUpdateAspectCreature_0))
		return;

	Thing groupThing = groupGetThing(eventMapX, eventMapY);
	/* If there is no creature at the location specified in the event then the event is ignored */
	if (groupThing == Thing::_endOfList)
		return;

	Group *curGroup = (Group *)_vm->_dungeonMan->getThingData(groupThing);
	CreatureInfo creatureInfo = _vm->_dungeonMan->_creatureInfos[curGroup->_type];
	/* Update the event */
	TimelineEvent nextEvent;
	setMapAndTime(nextEvent._mapTime, _vm->_dungeonMan->_currMapIndex, _vm->_gameTime);
	nextEvent._priority = 255 - creatureInfo._movementTicks; /* The fastest creatures (with small MovementTicks value) get higher event priority */
	nextEvent._B._location._mapX = eventMapX;
	nextEvent._B._location._mapY = eventMapY;
	/* If the creature is not on the party map then try and move the creature in a random direction and place a new event 37 in the timeline for the next creature movement */
	if (_vm->_dungeonMan->_currMapIndex != _vm->_dungeonMan->_partyMapIndex) {
		if (isMovementPossible(&creatureInfo, eventMapX, eventMapY, AL0446_i_Direction = _vm->getRandomNumber(4), false)) { /* BUG0_67 A group that is not on the party map may wrongly move or not move into a teleporter. Normally, a creature type with Wariness >= 10 (Vexirk, Materializer / Zytaz, Demon, Lord Chaos, Red Dragon / Dragon) would only move into a teleporter if the creature type is allowed on the destination map. However, the variable G0380_T_CurrentGroupThing identifying the group is not set before being used by F0139_DUNGEON_IsCreatureAllowedOnMap called by f202_isMovementPossible so the check to see if the creature type is allowed may operate on another creature type and thus return an incorrect result, causing the creature to teleport while it should not, or not to teleport while it should */
			AL0450_i_DestinationMapX = eventMapX;
			AL0451_i_DestinationMapY = eventMapY;
			AL0450_i_DestinationMapX += _vm->_dirIntoStepCountEast[AL0446_i_Direction];
			AL0451_i_DestinationMapY += _vm->_dirIntoStepCountNorth[AL0446_i_Direction];
			if (_vm->_moveSens->getMoveResult(groupThing, eventMapX, eventMapY, AL0450_i_DestinationMapX, AL0451_i_DestinationMapY))
				return;
			nextEvent._B._location._mapX = _vm->_moveSens->_moveResultMapX;
			nextEvent._B._location._mapY = _vm->_moveSens->_moveResultMapY;
		}
		nextEvent._type = k37_TMEventTypeUpdateBehaviourGroup;
		AL0446_i_Ticks = MAX(ABS(_vm->_dungeonMan->_currMapIndex - _vm->_dungeonMan->_partyMapIndex) << 4, creatureInfo._movementTicks << 1);
		/* BUG0_68 A group moves or acts with a wrong timing. Event is added below but L0465_s_NextEvent.C.Ticks has not been initialized. No consequence while the group is not on the party map. When the party enters the group map the first group event may have a wrong timing */
T0209005_AddEventAndReturn:
		nextEvent._mapTime += AL0446_i_Ticks;
		_vm->_timeline->addEventGetEventIndex(&nextEvent);
		return;
	}
	/* If the creature is Lord Chaos then ignore the event if the game is won. Initialize data to analyze Fluxcages */
	bool isArchEnemy = getFlag(creatureInfo._attributes, k0x2000_MaskCreatureInfo_archenemy);
	if (isArchEnemy) {
		if (_vm->_gameWon)
			return;

		_fluxCageCount = 0;
		_fluxCages[0] = 0;
	}
	ActiveGroup *activeGroup = &_activeGroups[curGroup->getActiveGroupIndex()];

	// CHECKME: Terrible mix of types
	int16 ticksSinceLastMove = (unsigned char)_vm->_gameTime - activeGroup->_lastMoveTime;
	if (ticksSinceLastMove < 0)
		ticksSinceLastMove += 256;

	int16 movementTicks = creatureInfo._movementTicks;
	if (movementTicks == k255_immobile)
		movementTicks = 100;

	if (_vm->_championMan->_party._freezeLifeTicks && !isArchEnemy) { /* If life is frozen and the creature is not Lord Chaos (Lord Chaos is immune to Freeze Life) then reschedule the event later (except for reactions which are ignored when life if frozen) */
		if (eventType < 0)
			return;
		nextEvent._type = eventType;
		nextEvent._C._ticks = ticks;
		AL0446_i_Ticks = 4; /* Retry in 4 ticks */
		goto T0209005_AddEventAndReturn;
	}
	/* If the specified event type is a 'reaction' instead of a real event from the timeline then create the corresponding reaction event with a delay:
	For event kM1_TMEventTypeCreateReactionEvent31ParyIsAdjacent, the reaction time is 1 tick
	For event kM2_TMEventTypeCreateReactionEvent30HitByProjectile and kM3_TMEventTypeCreateReactionEvent29DangerOnSquare, the reaction time may be 1 tick or slower: slow moving creatures react more slowly. The more recent is the last creature move, the slower the reaction */
	if (eventType < 0) {
		nextEvent._type = eventType + k32_TMEventTypeUpdateAspectGroup;
		if (eventType == kM1_TMEventTypeCreateReactionEvent31ParyIsAdjacent) {
			AL0446_i_Ticks = 1; /* Retry in 1 tick */
		} else {
			 AL0446_i_Ticks = ((movementTicks + 2) >> 2) - ticksSinceLastMove;
			 if (AL0446_i_Ticks < 1) /* AL0446_i_Ticks is the reaction time */
				 AL0446_i_Ticks = 1; /* Retry in 1 tick */
		}
		goto T0209005_AddEventAndReturn; /* BUG0_68 A group moves or acts with a wrong timing. Event is added but L0465_s_NextEvent.C.Ticks has not been initialized */
	}
	AL0447_i_Behavior = curGroup->getBehaviour();
	uint16 creatureCount = curGroup->getCount();
	int16 creatureSize = getFlag(creatureInfo._attributes, k0x0003_MaskCreatureInfo_size);
	AL0450_i_DistanceXToParty = ABS(eventMapX - _vm->_dungeonMan->_partyMapX);
	AL0451_i_DistanceYToParty = ABS(eventMapY - _vm->_dungeonMan->_partyMapY);
	_currentGroupMapX = eventMapX;
	_currentGroupMapY = eventMapY;
	_currGroupThing = groupThing;
	_groupMovementTestedDirections[0] = 0;
	_currGroupDistanceToParty = getDistanceBetweenSquares(eventMapX, eventMapY, _vm->_dungeonMan->_partyMapX, _vm->_dungeonMan->_partyMapY);
	_currGroupPrimaryDirToParty = getDirsWhereDestIsVisibleFromSource(eventMapX, eventMapY, _vm->_dungeonMan->_partyMapX, _vm->_dungeonMan->_partyMapY);
	_currGroupSecondaryDirToParty = _vm->_projexpl->_secondaryDirToOrFromParty;
	int32 nextAspectUpdateTime = 0;
	bool notUpdateBehaviorFl = true;
	bool newGroupDirectionFound;
	bool approachAfterReaction = false;
	bool moveToPriorLocation = false;
	int16 distanceToVisibleParty = 0;

	if (eventType <= k31_TMEventTypeGroupReactionPartyIsAdjecent) { /* Process Reaction events 29 to 31 */
		switch (eventType = eventType - k32_TMEventTypeUpdateAspectGroup) {
		case kM1_TMEventTypeCreateReactionEvent31ParyIsAdjacent: /* This event is used when the party bumps into a group or attacks a group physically (not with a spell). It causes the creature behavior to change to attack if it is not already attacking the party or fleeing from target */
			if ((AL0447_i_Behavior != k6_behavior_ATTACK) && (AL0447_i_Behavior != k5_behavior_FLEE)) {
				groupDeleteEvents(eventMapX, eventMapY);
				goto T0209044_SetBehavior6_Attack;
			}
			activeGroup->_targetMapX = _vm->_dungeonMan->_partyMapX;
			activeGroup->_targetMapY = _vm->_dungeonMan->_partyMapY;
			return;
		case kM2_TMEventTypeCreateReactionEvent30HitByProjectile: /* This event is used for the reaction of a group after a projectile impacted with one creature in the group (some creatures may have been killed) */
			if ((AL0447_i_Behavior == k6_behavior_ATTACK) || (AL0447_i_Behavior == k5_behavior_FLEE)) /* If the creature is attacking the party or fleeing from the target then there is no reaction */
				return;
			AL0446_i_Behavior2Or3 = ((AL0447_i_Behavior == k3_behavior_USELESS) || (AL0447_i_Behavior == k2_behavior_USELESS));
			if (AL0446_i_Behavior2Or3 || (_vm->getRandomNumber(4))) { /* BUG0_00 Useless code. Behavior cannot be 2 nor 3 because these values are never used. The actual condition is thus: if 3/4 chances */
				if (!groupGetDistanceToVisibleParty(curGroup, kM1_wholeCreatureGroup, eventMapX, eventMapY)) { /* If the group cannot see the party then look in a random direction to try and search for the party */
					approachAfterReaction = newGroupDirectionFound = false;
					goto T0209073_SetDirectionGroup;
				}
				if (AL0446_i_Behavior2Or3 || (_vm->getRandomNumber(4))) /* BUG0_00 Useless code. Behavior cannot be 2 nor 3 because these values are never used. The actual condition is thus: if 3/4 chances then no reaction */
					return;
			} /* No 'break': proceed to instruction after the next 'case' below. Reaction is to move in a random direction to try and avoid other projectiles */
		case kM3_TMEventTypeCreateReactionEvent29DangerOnSquare: /* This event is used when some creatures in the group were killed by a Poison Cloud or by a closing door or if Lord Chaos is surrounded by 3 Fluxcages. It causes the creature to move in a random direction to avoid the danger */
			approachAfterReaction = (AL0447_i_Behavior == k6_behavior_ATTACK); /* If the creature behavior is 'Attack' and it has to move to avoid danger then it will change its behavior to 'Approach' after the movement */
			newGroupDirectionFound = false;
			goto T0209058_MoveInRandomDirection;
		}
	}
	if (eventType < k37_TMEventTypeUpdateBehaviourGroup) { /* Process Update Aspect events 32 to 36 */
		nextEvent._type = eventType + 5;
		if (groupGetDistanceToVisibleParty(curGroup, kM1_wholeCreatureGroup, eventMapX, eventMapY)) {
			if ((AL0447_i_Behavior != k6_behavior_ATTACK) && (AL0447_i_Behavior != k5_behavior_FLEE)) {
				if (getDistance(_vm->_dungeonMan->_partyMapX, _vm->_dungeonMan->_partyMapY, eventMapX, eventMapY) <= 1)
					goto T0209044_SetBehavior6_Attack;
				if (((AL0447_i_Behavior == k0_behavior_WANDER) || (AL0447_i_Behavior == k3_behavior_USELESS)) && (AL0447_i_Behavior != k7_behavior_APPROACH)) /* BUG0_00 Useless code. Behavior cannot be 3 because this value is never used. Moreover, the second condition in the && is redundant (if the value is 0 or 3, it cannot be 7). The actual condition is: if (AL0447_i_Behavior == k0_behavior_WANDER) */
					goto T0209054_SetBehavior7_Approach;
			}
			activeGroup->_targetMapX = _vm->_dungeonMan->_partyMapX;
			activeGroup->_targetMapY = _vm->_dungeonMan->_partyMapY;
		}
		if (AL0447_i_Behavior == k6_behavior_ATTACK) {
			AL0446_i_CreatureAspectIndex = eventType - k33_TMEventTypeUpdateAspectCreature_0; /* Value -1 for event 32, meaning aspect will be updated for all creatures in the group */
			nextAspectUpdateTime = getCreatureAspectUpdateTime(activeGroup, AL0446_i_CreatureAspectIndex, getFlag(activeGroup->_aspect[AL0446_i_CreatureAspectIndex], k0x0080_MaskActiveGroupIsAttacking));
			goto T0209136;
		}
		if ((AL0450_i_DistanceXToParty > 3) || (AL0451_i_DistanceYToParty > 3)) {
			nextAspectUpdateTime = _vm->_gameTime + ((creatureInfo._animationTicks >> 4) & 0xF);
			goto T0209136;
		}
	} else { /* Process Update Behavior events 37 to 41 */
		int16 primaryDirectionToOrFromParty;
		notUpdateBehaviorFl = false;
		if (ticks)
			nextAspectUpdateTime = _vm->_gameTime;

		if (eventType == k37_TMEventTypeUpdateBehaviourGroup) { /* Process event 37, Update Group Behavior */
			bool allowMovementOverFakePitsAndFakeWalls;
			if ((AL0447_i_Behavior == k0_behavior_WANDER) || (AL0447_i_Behavior == k2_behavior_USELESS) || (AL0447_i_Behavior == k3_behavior_USELESS)) { /* BUG0_00 Useless code. Behavior cannot be 2 nor 3 because these values are never used. The actual condition is: if (AL0447_i_Behavior == k0_behavior_WANDER) */
				distanceToVisibleParty = groupGetDistanceToVisibleParty(curGroup, kM1_wholeCreatureGroup, eventMapX, eventMapY);
				if (distanceToVisibleParty) {
					if ((distanceToVisibleParty <= (creatureInfo.getAttackRange())) && ((!AL0450_i_DistanceXToParty) || (!AL0451_i_DistanceYToParty))) { /* If the creature is in range for attack and on the same row or column as the party on the map */
T0209044_SetBehavior6_Attack:
						if (eventType == kM2_TMEventTypeCreateReactionEvent30HitByProjectile) {
							groupDeleteEvents(eventMapX, eventMapY);
						}
						activeGroup->_targetMapX = _vm->_dungeonMan->_partyMapX;
						activeGroup->_targetMapY = _vm->_dungeonMan->_partyMapY;
						curGroup->setBehaviour(k6_behavior_ATTACK);
						AL0446_i_Direction = _currGroupPrimaryDirToParty;
						for (AL0447_i_CreatureIndex = creatureCount; AL0447_i_CreatureIndex >= 0; AL0447_i_CreatureIndex--) {
							if ((getCreatureValue(activeGroup->_directions, AL0447_i_CreatureIndex) != AL0446_i_Direction) &&
								((!AL0447_i_CreatureIndex) || (!_vm->getRandomNumber(2)))) {
								setGroupDirection(activeGroup, AL0446_i_Direction, AL0447_i_CreatureIndex, creatureCount && (creatureSize == k1_MaskCreatureSizeHalf));
								M32_setTime(nextEvent._mapTime, _vm->_gameTime + _vm->getRandomNumber(4) + 2); /* Random delay represents the time for the creature to turn */
							} else {
								M32_setTime(nextEvent._mapTime, _vm->_gameTime + 1);
							}
							if (notUpdateBehaviorFl) {
								nextEvent._mapTime += MIN((uint16)((creatureInfo._attackTicks >> 1) + _vm->getRandomNumber(4)), ticks);
							}
							nextEvent._type = k38_TMEventTypeUpdateBehaviour_0 + AL0447_i_CreatureIndex;
							addGroupEvent(&nextEvent, getCreatureAspectUpdateTime(activeGroup, AL0447_i_CreatureIndex, false));
						}
						return;
					}
					if (AL0447_i_Behavior != k2_behavior_USELESS) { /* BUG0_00 Useless code. Behavior cannot be 2 because this value is never used */
T0209054_SetBehavior7_Approach:
						curGroup->setBehaviour(k7_behavior_APPROACH);
						activeGroup->_targetMapX = _vm->_dungeonMan->_partyMapX;
						activeGroup->_targetMapY = _vm->_dungeonMan->_partyMapY;
						nextEvent._mapTime += 1;
						goto T0209134_SetEvent37;
					}
				} else {
					if (AL0447_i_Behavior == k0_behavior_WANDER) {
						primaryDirectionToOrFromParty = getSmelledPartyPrimaryDirOrdinal(&creatureInfo, eventMapX, eventMapY);
						if (primaryDirectionToOrFromParty) {
							primaryDirectionToOrFromParty--;
							allowMovementOverFakePitsAndFakeWalls = false;
							goto T0209085_SingleSquareMove;
						}
						newGroupDirectionFound = false;
						if (_vm->getRandomNumber(2)) {
T0209058_MoveInRandomDirection:
							AL0446_i_Direction = _vm->getRandomNumber(4);
							AL0447_i_ReferenceDirection = AL0446_i_Direction;
							do {
								AL0450_i_DestinationMapX = eventMapX;
								AL0451_i_DestinationMapY = eventMapY;
								AL0450_i_DestinationMapX += _vm->_dirIntoStepCountEast[AL0446_i_Direction], AL0451_i_DestinationMapY += _vm->_dirIntoStepCountNorth[AL0446_i_Direction];
								if (((activeGroup->_priorMapX != AL0450_i_DestinationMapX) ||
									(activeGroup->_priorMapY != AL0451_i_DestinationMapY) ||
									 (moveToPriorLocation = !_vm->getRandomNumber(4))) /* 1/4 chance of moving back to the square that the creature comes from */
									&& isMovementPossible(&creatureInfo, eventMapX, eventMapY, AL0446_i_Direction, false)) {
T0209061_MoveGroup:
									AL0447_i_Ticks = (movementTicks >> 1) - ticksSinceLastMove;
									newGroupDirectionFound = (AL0447_i_Ticks <= 0);
									if (newGroupDirectionFound) {
										if (_vm->_moveSens->getMoveResult(groupThing, eventMapX, eventMapY, AL0450_i_DestinationMapX, AL0451_i_DestinationMapY))
											return;
										nextEvent._B._location._mapX = _vm->_moveSens->_moveResultMapX;
										nextEvent._B._location._mapY = _vm->_moveSens->_moveResultMapY;;
										activeGroup->_priorMapX = eventMapX;
										activeGroup->_priorMapY = eventMapY;
										activeGroup->_lastMoveTime = _vm->_gameTime;
									} else {
										movementTicks = AL0447_i_Ticks;
										ticksSinceLastMove = -1;
									}
									break;
								}
								if (_groupMovementBlockedByParty) {
									if ((eventType != kM3_TMEventTypeCreateReactionEvent29DangerOnSquare) &&
										((curGroup->getBehaviour() != k5_behavior_FLEE) ||
										 !getFirstPossibleMovementDirOrdinal(&creatureInfo, eventMapX, eventMapY, false) ||
										 _vm->getRandomNumber(2)))
										goto T0209044_SetBehavior6_Attack;
									activeGroup->_targetMapX = _vm->_dungeonMan->_partyMapX;
									activeGroup->_targetMapY = _vm->_dungeonMan->_partyMapY;
								}
								AL0446_i_Direction = returnNextVal(AL0446_i_Direction);
							} while (AL0446_i_Direction != AL0447_i_ReferenceDirection);
						}
						if (!newGroupDirectionFound &&
							(ticksSinceLastMove != -1) &&
							isArchEnemy &&
							((eventType == kM3_TMEventTypeCreateReactionEvent29DangerOnSquare) || !_vm->getRandomNumber(4))) { /* BUG0_15 The game hangs when you close a door on Lord Chaos. A condition is missing in the code to manage creatures and this may create an infinite loop between two parts in the code */
							_vm->_projexpl->_secondaryDirToOrFromParty = returnNextVal(primaryDirectionToOrFromParty = _vm->getRandomNumber(4));
							goto T0209089_DoubleSquareMove; /* BUG0_69 Memory corruption when you close a door on Lord Chaos. The local variable (L0454_i_PrimaryDirectionToOrFromParty) containing the direction where Lord Chaos tries to move may be used as an array index without being initialized and cause memory corruption */
						}
						if (newGroupDirectionFound || ((!_vm->getRandomNumber(4) || (distanceToVisibleParty <= creatureInfo.getSmellRange())) && (eventType != kM3_TMEventTypeCreateReactionEvent29DangerOnSquare))) {
T0209073_SetDirectionGroup:
							if (!newGroupDirectionFound && (ticksSinceLastMove >= 0)) { /* If direction is not found yet then look around in a random direction */
								AL0446_i_Direction = _vm->getRandomNumber(4);
							}
							setDirGroup(activeGroup, AL0446_i_Direction, creatureCount, creatureSize);
						}
						/* If event is kM3_TMEventTypeCreateReactionEvent29DangerOnSquare or kM2_TMEventTypeCreateReactionEvent30HitByProjectile */
						if (eventType < kM1_TMEventTypeCreateReactionEvent31ParyIsAdjacent) {
							if (!newGroupDirectionFound)
								return;
							if (approachAfterReaction)
								curGroup->setBehaviour(k7_behavior_APPROACH);

							stopAttacking(activeGroup, eventMapX, eventMapY);
						}
					}
				}
			} else {
				if (AL0447_i_Behavior == k7_behavior_APPROACH) {
					distanceToVisibleParty = groupGetDistanceToVisibleParty(curGroup, kM1_wholeCreatureGroup, eventMapX, eventMapY);
					if (distanceToVisibleParty) {
						if ((distanceToVisibleParty <= creatureInfo.getAttackRange()) && ((!AL0450_i_DistanceXToParty) || (!AL0451_i_DistanceYToParty))) /* If the creature is in range for attack and on the same row or column as the party on the map */
							goto T0209044_SetBehavior6_Attack;
T0209081_RunTowardParty:
						movementTicks++;
						movementTicks = movementTicks >> 1; /* Running speed is half the movement ticks */
						AL0450_i_TargetMapX = (activeGroup->_targetMapX = _vm->_dungeonMan->_partyMapX);
						AL0451_i_TargetMapY = (activeGroup->_targetMapY = _vm->_dungeonMan->_partyMapY);
					} else {
T0209082_WalkTowardTarget:
						AL0450_i_TargetMapX = activeGroup->_targetMapX;
						AL0451_i_TargetMapY = activeGroup->_targetMapY;
						/* If the creature reached its target but the party is not there anymore */
						if ((eventMapX == AL0450_i_TargetMapX) && (eventMapY == AL0451_i_TargetMapY)) {
							newGroupDirectionFound = false;
							curGroup->setBehaviour(k0_behavior_WANDER);
							goto T0209073_SetDirectionGroup;
						}
					}
					allowMovementOverFakePitsAndFakeWalls = true;
T0209084_SingleSquareMoveTowardParty:
					primaryDirectionToOrFromParty = getDirsWhereDestIsVisibleFromSource(eventMapX, eventMapY, AL0450_i_TargetMapX, AL0451_i_TargetMapY);
T0209085_SingleSquareMove:
					if (isMovementPossible(&creatureInfo, eventMapX, eventMapY, AL0446_i_Direction = primaryDirectionToOrFromParty, allowMovementOverFakePitsAndFakeWalls) ||
						isMovementPossible(&creatureInfo, eventMapX, eventMapY, AL0446_i_Direction = _vm->_projexpl->_secondaryDirToOrFromParty, allowMovementOverFakePitsAndFakeWalls && _vm->getRandomNumber(2)) ||
						isMovementPossible(&creatureInfo, eventMapX, eventMapY, AL0446_i_Direction = returnOppositeDir((Direction)AL0446_i_Direction), false) ||
						(!_vm->getRandomNumber(4) && isMovementPossible(&creatureInfo, eventMapX, eventMapY, AL0446_i_Direction = returnOppositeDir((Direction)primaryDirectionToOrFromParty), false))) {
						AL0450_i_DestinationMapX = eventMapX;
						AL0451_i_DestinationMapY = eventMapY;
						AL0450_i_DestinationMapX += _vm->_dirIntoStepCountEast[AL0446_i_Direction], AL0451_i_DestinationMapY += _vm->_dirIntoStepCountNorth[AL0446_i_Direction];
						goto T0209061_MoveGroup;
					}
					if (isArchEnemy) {
T0209089_DoubleSquareMove:
						getFirstPossibleMovementDirOrdinal(&creatureInfo, eventMapX, eventMapY, false); /* BUG0_00 Useless code. Returned value is ignored. When Lord Chaos teleports two squares away the ability to move to the first square is ignored which means Lord Chaos can teleport through walls or any other obstacle */
						if (isArchenemyDoubleMovementPossible(&creatureInfo, eventMapX, eventMapY, AL0446_i_Direction = primaryDirectionToOrFromParty) ||
							isArchenemyDoubleMovementPossible(&creatureInfo, eventMapX, eventMapY, AL0446_i_Direction = _vm->_projexpl->_secondaryDirToOrFromParty) ||
							(_fluxCageCount && isArchenemyDoubleMovementPossible(&creatureInfo, eventMapX, eventMapY, AL0446_i_Direction = returnOppositeDir((Direction)AL0446_i_Direction))) ||
							((_fluxCageCount >= 2) && isArchenemyDoubleMovementPossible(&creatureInfo, eventMapX, eventMapY, AL0446_i_Direction = returnOppositeDir((Direction)primaryDirectionToOrFromParty)))) {
							AL0450_i_DestinationMapX = eventMapX;
							AL0451_i_DestinationMapY = eventMapY;
							AL0450_i_DestinationMapX += _vm->_dirIntoStepCountEast[AL0446_i_Direction] * 2, AL0451_i_DestinationMapY += _vm->_dirIntoStepCountNorth[AL0446_i_Direction] * 2;
							_vm->_sound->requestPlay(k17_soundBUZZ, AL0450_i_DestinationMapX, AL0451_i_DestinationMapY, k1_soundModePlayIfPrioritized);
							goto T0209061_MoveGroup;
						}
					}
					setDirGroup(activeGroup, primaryDirectionToOrFromParty, creatureCount, creatureSize);
				} else {
					if (AL0447_i_Behavior == k5_behavior_FLEE) {
T0209094_FleeFromTarget:
						allowMovementOverFakePitsAndFakeWalls = true;
						/* If the creature can see the party then update target coordinates */
						distanceToVisibleParty = groupGetDistanceToVisibleParty(curGroup, kM1_wholeCreatureGroup, eventMapX, eventMapY);
						if (distanceToVisibleParty) {
							AL0450_i_TargetMapX = (activeGroup->_targetMapX = _vm->_dungeonMan->_partyMapX);
							AL0451_i_TargetMapY = (activeGroup->_targetMapY = _vm->_dungeonMan->_partyMapY);
						} else {
							if (!(--(activeGroup->_delayFleeingFromTarget))) { /* If the creature is not afraid anymore then stop fleeing from target */
T0209096_SetBehavior0_Wander:
								newGroupDirectionFound = false;
								curGroup->setBehaviour(k0_behavior_WANDER);
								goto T0209073_SetDirectionGroup;
							}
							if (_vm->getRandomNumber(2)) {
								/* If the creature cannot move and the party is adjacent then stop fleeing */
								if (!getFirstPossibleMovementDirOrdinal(&creatureInfo, eventMapX, eventMapY, false)) {
									if (getDistance(eventMapX, eventMapY, _vm->_dungeonMan->_partyMapX, _vm->_dungeonMan->_partyMapY) <= 1)
										goto T0209096_SetBehavior0_Wander;
								}
								/* Set creature target to the home square where the creature was located when the party entered the map */
								AL0450_i_TargetMapX = activeGroup->_homeMapX;
								AL0451_i_TargetMapY = activeGroup->_homeMapY;
								goto T0209084_SingleSquareMoveTowardParty;
							}
							AL0450_i_TargetMapX = activeGroup->_targetMapX;
							AL0451_i_TargetMapY = activeGroup->_targetMapY;
						}
						/* Try and flee from the party (opposite direction) */
						primaryDirectionToOrFromParty = returnOppositeDir((Direction)getDirsWhereDestIsVisibleFromSource(eventMapX, eventMapY, AL0450_i_TargetMapX, AL0451_i_TargetMapY));
						_vm->_projexpl->_secondaryDirToOrFromParty = returnOppositeDir((Direction)_vm->_projexpl->_secondaryDirToOrFromParty);
						movementTicks -= (movementTicks >> 2);
						goto T0209085_SingleSquareMove;
					}
				}
			}
		} else { /* Process events 38 to 41, Update Creature Behavior */
			if (AL0447_i_Behavior == k5_behavior_FLEE) {
				if (creatureCount) {
					stopAttacking(activeGroup, eventMapX, eventMapY);
				}
				goto T0209094_FleeFromTarget;
			}
			/* If the creature is attacking, then compute the next aspect update time and the next attack time */
			if (getFlag(activeGroup->_aspect[AL0447_i_CreatureIndex = eventType - k38_TMEventTypeUpdateBehaviour_0], k0x0080_MaskActiveGroupIsAttacking)) {
				nextAspectUpdateTime = getCreatureAspectUpdateTime(activeGroup, AL0447_i_CreatureIndex, false);
				nextEvent._mapTime += ((AL0447_i_Ticks = creatureInfo._attackTicks) + _vm->getRandomNumber(4) - 1);
				if (AL0447_i_Ticks > 15)
					nextEvent._mapTime += _vm->getRandomNumber(8) - 2;
			} else { /* If the creature is not attacking, then try attacking if possible */
				if (AL0447_i_CreatureIndex > creatureCount) /* Ignore event if it is for a creature that is not in the group */
					return;

				primaryDirectionToOrFromParty = _currGroupPrimaryDirToParty;
				distanceToVisibleParty = groupGetDistanceToVisibleParty(curGroup, AL0447_i_CreatureIndex, eventMapX, eventMapY);
				/* If the party is visible, update the target coordinates */
				if (distanceToVisibleParty) {
					activeGroup->_targetMapX = _vm->_dungeonMan->_partyMapX;
					activeGroup->_targetMapY = _vm->_dungeonMan->_partyMapY;
				}
				/* If there is a single creature in the group that is not full square sized and 1/4 chance */
				if (!creatureCount && (creatureSize != k2_MaskCreatureSizeFull) && !((AL0446_i_GroupCellsCriteria = _vm->getRandomNumber(65536)) & 0x00C0)) {
					if (activeGroup->_cells != k255_CreatureTypeSingleCenteredCreature) {
						/* If the creature is not already on the center of the square then change its cell */
						if (AL0446_i_GroupCellsCriteria & 0x0038) { /* 7/8 chances of changing cell to the center of the square */
							activeGroup->_cells = k255_CreatureTypeSingleCenteredCreature;
						} else { /* 1/8 chance of changing cell to the next or previous cell on the square */
							AL0446_i_GroupCellsCriteria = normalizeModulo4(normalizeModulo4(activeGroup->_cells) + ((AL0446_i_GroupCellsCriteria & 0x0001) ? 1 : -1));
						}
					}
					/* If 1/8 chance and the creature is not adjacent to the party and is a quarter square sized creature then process projectile impacts and update the creature cell if still alive. When the creature is not in front of the party, it has 7/8 chances of dodging a projectile by moving to another cell or staying in the center of the square */
					if (!(AL0446_i_GroupCellsCriteria & 0x0038) && (distanceToVisibleParty != 1) && (creatureSize == k0_MaskCreatureSizeQuarter)) {
						if (_vm->_projexpl->projectileGetImpactCount(kM1_CreatureElemType, eventMapX, eventMapY, activeGroup->_cells) && (_vm->_projexpl->_creatureDamageOutcome == k2_outcomeKilledAllCreaturesInGroup)) /* This call to F0218_PROJECTILE_GetImpactCount works fine because there is a single creature in the group so L0445_ps_ActiveGroup->Cells contains only one cell index */
							return;
						activeGroup->_cells = normalizeModulo4(AL0446_i_GroupCellsCriteria);
					}
				}
				/* If the creature can see the party and is looking in the party direction or can attack in all direction */
				if (distanceToVisibleParty &&
					(getFlag(creatureInfo._attributes, k0x0004_MaskCreatureInfo_sideAttack) ||
					 getCreatureValue(activeGroup->_directions, AL0447_i_CreatureIndex) == primaryDirectionToOrFromParty)) {
					/* If the creature is in range to attack the party and random test succeeds */
					if ((distanceToVisibleParty <= (AL0446_i_Range = creatureInfo.getAttackRange())) &&
						(!AL0450_i_DistanceXToParty || !AL0451_i_DistanceYToParty) &&
						(AL0446_i_Range <= (_vm->getRandomNumber(16) + 1))) {
						if ((AL0446_i_Range == 1) &&
							(!getFlag(AL0446_i_CreatureAttributes = creatureInfo._attributes, k0x0008_MaskCreatureInfo_preferBackRow) || !_vm->getRandomNumber(4) || !getFlag(AL0446_i_CreatureAttributes, k0x0010_MaskCreatureInfo_attackAnyChamp)) &&
							(creatureSize == k0_MaskCreatureSizeQuarter) &&
							(activeGroup->_cells != k255_CreatureTypeSingleCenteredCreature) &&
							((AL0446_i_Cell = getCreatureValue(activeGroup->_cells, AL0447_i_CreatureIndex)) != primaryDirectionToOrFromParty) &&
							(AL0446_i_Cell != returnNextVal(primaryDirectionToOrFromParty))) { /* If the creature cannot cast spells (range = 1) and is not on a cell where it can attack the party directly and is a quarter square sized creature not in the center of the square then the creature moves to another cell and attack does not occur immediately */
							if (!creatureCount && _vm->getRandomNumber(2)) {
								activeGroup->_cells = k255_CreatureTypeSingleCenteredCreature;
							} else {
								if ((primaryDirectionToOrFromParty & 0x0001) == (AL0446_i_Cell & 0x0001)) {
									AL0446_i_Cell--;
								} else {
									AL0446_i_Cell++;
								}
								if (!getCreatureOrdinalInCell(curGroup, AL0446_i_Cell = normalizeModulo4(AL0446_i_Cell)) ||
									(_vm->getRandomNumber(2) && !getCreatureOrdinalInCell(curGroup, AL0446_i_Cell = returnOppositeDir((Direction)AL0446_i_Cell)))) { /* If the selected cell (or the opposite cell) is not already occupied by a creature */
									if (_vm->_projexpl->projectileGetImpactCount(kM1_CreatureElemType, eventMapX, eventMapY, activeGroup->_cells) && (_vm->_projexpl->_creatureDamageOutcome == k2_outcomeKilledAllCreaturesInGroup)) /* BUG0_70 A projectile impact on a creature may be ignored. The function F0218_PROJECTILE_GetImpactCount to detect projectile impacts when a quarter square sized creature moves inside a group (to another cell on the same square) may fail if there are several creatures in the group because the function expects a single cell index for its last parameter. The function should be called once for each cell where there is a creature */
										return;
									if (_vm->_projexpl->_creatureDamageOutcome != k1_outcomeKilledSomeCreaturesInGroup) {
										activeGroup->_cells = getGroupValueUpdatedWithCreatureValue(activeGroup->_cells, AL0447_i_CreatureIndex, AL0446_i_Cell);
									}
								}
							}
							nextEvent._mapTime += MAX(1, (creatureInfo._movementTicks >> 1) + _vm->getRandomNumber(2)); /* Time for the creature to change cell */
							nextEvent._type = eventType;
							goto T0209135;
						}
						nextAspectUpdateTime = getCreatureAspectUpdateTime(activeGroup, AL0447_i_CreatureIndex, isCreatureAttacking(curGroup, eventMapX, eventMapY, AL0447_i_CreatureIndex));
						nextEvent._mapTime += (creatureInfo._animationTicks & 0xF) + _vm->getRandomNumber(2);
					} else {
						curGroup->setBehaviour(k7_behavior_APPROACH);
						if (creatureCount) {
							stopAttacking(activeGroup, eventMapX, eventMapY);
						}
						goto T0209081_RunTowardParty;
					}
				} else {
					/* If the party is visible, update target coordinates */
					if (groupGetDistanceToVisibleParty(curGroup, kM1_wholeCreatureGroup, eventMapX, eventMapY)) {
						activeGroup->_targetMapX = _vm->_dungeonMan->_partyMapX;
						activeGroup->_targetMapY = _vm->_dungeonMan->_partyMapY;
						setGroupDirection(activeGroup, primaryDirectionToOrFromParty, AL0447_i_CreatureIndex, creatureCount && (creatureSize == k1_MaskCreatureSizeHalf));
						nextEvent._mapTime += 2;
						nextAspectUpdateTime = filterTime(nextEvent._mapTime);
					} else { /* If the party is not visible, move to the target (last known party location) */
						curGroup->setBehaviour(k7_behavior_APPROACH);
						if (creatureCount) {
							stopAttacking(activeGroup, eventMapX, eventMapY);
						}
						goto T0209082_WalkTowardTarget;
					}
				}
			}
			nextEvent._type = eventType;
			goto T0209136;
		}
		nextEvent._mapTime += MAX(1, _vm->getRandomNumber(4) + movementTicks - 1);
T0209134_SetEvent37:
		nextEvent._type = k37_TMEventTypeUpdateBehaviourGroup;
	}
T0209135:
	if (!nextAspectUpdateTime) {
		nextAspectUpdateTime = getCreatureAspectUpdateTime(activeGroup, kM1_wholeCreatureGroup, false);
	}
T0209136:
	if (notUpdateBehaviorFl) {
		nextEvent._mapTime += ticks;
	} else {
		nextAspectUpdateTime += ticks;
	}
	addGroupEvent(&nextEvent, nextAspectUpdateTime);
}

bool GroupMan::isMovementPossible(CreatureInfo *creatureInfo, int16 mapX, int16 mapY, uint16 dir, bool allowMovementOverImaginaryPitsAndFakeWalls) {
	_groupMovementTestedDirections[dir] = true;
	_groupMovementBlockedByGroupThing = Thing::_endOfList;
	_groupMovementBlockedByDoor = false;
	_groupMovementBlockedByParty = false;
	if (creatureInfo->_movementTicks == k255_immobile)
		return false;

	_vm->_dungeonMan->mapCoordsAfterRelMovement((Direction)dir, 1, 0, mapX, mapY);
	uint16 curSquare = _vm->_dungeonMan->_currMapData[mapX][mapY];
	int16 curSquareType = Square(curSquare).getType();
	_groupMovBlockedByWallStairsPitFakeWalFluxCageTeleporter =
		!(((mapX >= 0) && (mapX < _vm->_dungeonMan->_currMapWidth)) &&
		 ((mapY >= 0) && (mapY < _vm->_dungeonMan->_currMapHeight)) &&
		  (curSquareType != k0_ElementTypeWall) &&
		  (curSquareType != k3_ElementTypeStairs) &&
		 ((curSquareType != k2_ElementTypePit) || (getFlag(curSquare, k0x0001_PitImaginary) && allowMovementOverImaginaryPitsAndFakeWalls) || !getFlag(curSquare, k0x0008_PitOpen) || getFlag(creatureInfo->_attributes, k0x0020_MaskCreatureInfo_levitation)) &&
		 ((curSquareType != k6_ElementTypeFakeWall) || getFlag(curSquare, k0x0004_FakeWallOpen) || (getFlag(curSquare, k0x0001_FakeWallImaginary) && allowMovementOverImaginaryPitsAndFakeWalls)));

	if (_groupMovBlockedByWallStairsPitFakeWalFluxCageTeleporter)
		return false;

	if (getFlag(creatureInfo->_attributes, k0x2000_MaskCreatureInfo_archenemy)) {
		Thing curThing = _vm->_dungeonMan->getSquareFirstThing(mapX, mapY);
		while (curThing != Thing::_endOfList) {
			if ((curThing).getType() == k15_ExplosionThingType) {
				Teleporter *curTeleporter = (Teleporter *)_vm->_dungeonMan->getThingData(curThing);
				if (((Explosion *)curTeleporter)->setType(k50_ExplosionType_Fluxcage)) {
					_fluxCages[dir] = true;
					_fluxCageCount++;
					_groupMovBlockedByWallStairsPitFakeWalFluxCageTeleporter = true;
					return false;
				}
			}
			curThing = _vm->_dungeonMan->getNextThing(curThing);
		}
	}
	if ((curSquareType == k5_ElementTypeTeleporter) && getFlag(curSquare, k0x0008_TeleporterOpen) && (creatureInfo->getWariness() >= 10)) {
		Teleporter *curTeleporter = (Teleporter *)_vm->_dungeonMan->getSquareFirstThingData(mapX, mapY);
		if (getFlag(curTeleporter->getScope(), k0x0001_TelepScopeCreatures) && !_vm->_dungeonMan->isCreatureAllowedOnMap(_currGroupThing, curTeleporter->getTargetMapIndex())) {
			_groupMovBlockedByWallStairsPitFakeWalFluxCageTeleporter = true;
			return false;
		}
	}

	_groupMovementBlockedByParty = (_vm->_dungeonMan->_currMapIndex == _vm->_dungeonMan->_partyMapIndex) && (mapX == _vm->_dungeonMan->_partyMapX) && (mapY == _vm->_dungeonMan->_partyMapY);
	if (_groupMovementBlockedByParty)
		return false;

	if (curSquareType == k4_DoorElemType) {
		Teleporter *curTeleporter = (Teleporter *)_vm->_dungeonMan->getSquareFirstThingData(mapX, mapY);
		if (((Square(curSquare).getDoorState()) > (((Door *)curTeleporter)->opensVertically() ? CreatureInfo::getHeight(creatureInfo->_attributes) : 1)) && ((Square(curSquare).getDoorState()) != k5_doorState_DESTROYED) && !getFlag(creatureInfo->_attributes, k0x0040_MaskCreatureInfo_nonMaterial)) {
			_groupMovementBlockedByDoor = true;
			return false;
		}
	}

	_groupMovementBlockedByGroupThing = groupGetThing(mapX, mapY);
	return (_groupMovementBlockedByGroupThing == Thing::_endOfList);
}

int16 GroupMan::getDistanceBetweenSquares(int16 srcMapX, int16 srcMapY, int16 destMapX, int16 destMapY) {
	return ABS(srcMapX - destMapX) + ABS(srcMapY - destMapY);
}

int16 GroupMan::groupGetDistanceToVisibleParty(Group *group, int16 creatureIndex, int16 mapX, int16 mapY) {
	uint16 groupDirections;
	CreatureInfo *groupCreatureInfo = &_vm->_dungeonMan->_creatureInfos[group->_type];
	if (_vm->_championMan->_party._event71Count_Invisibility && !getFlag(groupCreatureInfo->_attributes, k0x0800_MaskCreatureInfo_seeInvisible))
		return 0;

	bool alwaysSee = false;
	int16 checkDirectionsCount; /* Count of directions to test in L0425_ai_CreatureViewDirections */
	int16 creatureViewDirections[4]; /* List of directions to test */
	if (getFlag(groupCreatureInfo->_attributes, k0x0004_MaskCreatureInfo_sideAttack)) { /* If creature can see in all directions */
		alwaysSee = true;
		checkDirectionsCount = 1;
		creatureViewDirections[0] = kDirNorth;
	} else {
		groupDirections = _activeGroups[group->getActiveGroupIndex()]._directions;
		if (creatureIndex < 0) { /* Negative index means test if each creature in the group can see the party in their respective direction */
			checkDirectionsCount = 0;
			for (creatureIndex = group->getCount(); creatureIndex >= 0; creatureIndex--) {
				int16 creatureDirection = normalizeModulo4(groupDirections >> (creatureIndex << 1));
				int16 counter = checkDirectionsCount;
				bool skipSet = false;
				while (counter--) {
					if (creatureViewDirections[counter] == creatureDirection) { /* If the creature looks in the same direction as another one in the group */
						skipSet = true;
						break;
					}
				}
				if (!skipSet)
					creatureViewDirections[checkDirectionsCount++] = creatureDirection;
			}
		} else { /* Positive index means test only if the specified creature in the group can see the party in its direction */
			creatureViewDirections[0] = getCreatureValue(groupDirections, creatureIndex);
			checkDirectionsCount = 1;
		}
	}

	while (checkDirectionsCount--) {
		if (alwaysSee || isDestVisibleFromSource(creatureViewDirections[checkDirectionsCount], mapX, mapY, _vm->_dungeonMan->_partyMapX, _vm->_dungeonMan->_partyMapY)) {
			int16 sightRange = groupCreatureInfo->getSightRange();
			if (!getFlag(groupCreatureInfo->_attributes, k0x1000_MaskCreatureInfo_nightVision))
				sightRange -= _vm->_displayMan->_dungeonViewPaletteIndex >> 1;

			if (_currGroupDistanceToParty > MAX<int16>(1, sightRange))
				return 0;

			return getDistanceBetweenUnblockedSquares(mapX, mapY, _vm->_dungeonMan->_partyMapX, _vm->_dungeonMan->_partyMapY, &GroupMan::isViewPartyBlocked);
		}
	}
	return 0;
}

int16 GroupMan::getDistanceBetweenUnblockedSquares(int16 srcMapX, int16 srcMapY,
													int16 destMapX, int16 destMapY, bool (GroupMan::*isBlocked)(uint16, uint16)) {

	if (getDistance(srcMapX, srcMapY, destMapX, destMapY) <= 1) {
		return 1;
	}

	int16 distanceX = ABS(destMapX - srcMapX);
	int16 distanceY = ABS(destMapY - srcMapY);
	bool isDistanceXSmallerThanDistanceY = (distanceX < distanceY);
	bool isDistanceXEqualsDistanceY = (distanceX == distanceY);
	int16 pathMapX = destMapX;
	int16 pathMapY = destMapY;
	int16 axisStepX = ((pathMapX - srcMapX) > 0) ? -1 : 1;
	int16 axisStepY = ((pathMapY - srcMapY) > 0) ? -1 : 1;
	int16 largestAxisDistance;

	int16 valueA;
	int16 valueB;
	int16 valueC;

	if (isDistanceXSmallerThanDistanceY) {
		largestAxisDistance = pathMapY - srcMapY;
		valueC = (largestAxisDistance ? ((pathMapX - srcMapX) << 6) / largestAxisDistance : 128);
	} else {
		largestAxisDistance = pathMapX - srcMapX;
		valueC = (largestAxisDistance ? ((pathMapY - srcMapY) << 6) / largestAxisDistance : 128);
	}

	/* 128 when the creature is on the same row or column as the party */
	do {
		if (isDistanceXEqualsDistanceY) {
			if ((   (CALL_MEMBER_FN(*_vm->_groupMan, isBlocked))(pathMapX + axisStepX, pathMapY) 
				 && (CALL_MEMBER_FN(*_vm->_groupMan, isBlocked))(pathMapX, pathMapY + axisStepY)) 
			 || (CALL_MEMBER_FN(*_vm->_groupMan, isBlocked))(pathMapX = pathMapX + axisStepX, pathMapY = pathMapY + axisStepY))
				return 0;
		} else {
			if (isDistanceXSmallerThanDistanceY) {
				valueA = ABS(((pathMapY - srcMapY) ? ((pathMapX + axisStepX - srcMapX) << 6) / largestAxisDistance : 128) - valueC);
				valueB = ABS(((pathMapY + axisStepY - srcMapY) ? ((pathMapX - srcMapX) << 6) / largestAxisDistance : 128) - valueC);
			} else {
				valueA = ABS(((pathMapX + axisStepX - srcMapX) ? ((pathMapY - srcMapY) << 6) / largestAxisDistance : 128) - valueC);
				valueB = ABS(((pathMapX - srcMapX) ? ((pathMapY + axisStepY - srcMapY) << 6) / largestAxisDistance : 128) - valueC);
			}

			if (valueA < valueB)
				pathMapX += axisStepX;
			else
				pathMapY += axisStepY;

			if ((CALL_MEMBER_FN(*_vm->_groupMan, isBlocked))(pathMapX, pathMapY)) {
				pathMapX += axisStepX;
				pathMapY -= axisStepY;
				if (((valueA != valueB) || (CALL_MEMBER_FN(*_vm->_groupMan, isBlocked))(pathMapX, pathMapY)))
					return 0;
			}
		}
	} while (getDistance(pathMapX, pathMapY, srcMapX, srcMapY) > 1);
	return getDistanceBetweenSquares(srcMapX, srcMapY, destMapX, destMapY);
}

bool GroupMan::isViewPartyBlocked(uint16 mapX, uint16 mapY) {
	uint16 curSquare = _vm->_dungeonMan->_currMapData[mapX][mapY];
	int16 curSquareType = Square(curSquare).getType();
	if (curSquareType == k4_DoorElemType) {
		Door *curDoor = (Door *)_vm->_dungeonMan->getSquareFirstThingData(mapX, mapY);
		int16 curDoorState = Square(curSquare).getDoorState();
		return ((curDoorState == k3_doorState_FOURTH) || (curDoorState == k4_doorState_CLOSED)) && !getFlag(_vm->_dungeonMan->_currMapDoorInfo[curDoor->getType()]._attributes, k0x0001_MaskDoorInfo_CraturesCanSeeThrough);
	}
	return (curSquareType == k0_ElementTypeWall) || ((curSquareType == k6_ElementTypeFakeWall) && !getFlag(curSquare, k0x0004_FakeWallOpen));
}

int32 GroupMan::getCreatureAspectUpdateTime(ActiveGroup *activeGroup, int16 creatureIndex, bool isAttacking) {
	uint16 L0326_ui_Multiple;
#define AL0326_ui_Aspect         L0326_ui_Multiple
#define AL0326_ui_AnimationTicks L0326_ui_Multiple
	uint16 L0327_ui_CreatureGraphicInfo;
	int16 L0328_i_Offset;
	Group *L0329_ps_Group;
	bool L0330_B_ProcessGroup;
	uint16 L0331_ui_CreatureType;
	uint16 L1635_ui_SoundIndex;

	L0329_ps_Group = &(((Group *)_vm->_dungeonMan->_thingData[k4_GroupThingType])[activeGroup->_groupThingIndex]);
	L0327_ui_CreatureGraphicInfo = _vm->_dungeonMan->_creatureInfos[L0331_ui_CreatureType = L0329_ps_Group->_type]._graphicInfo;
	L0330_B_ProcessGroup = (creatureIndex < 0);
	if (L0330_B_ProcessGroup) /* If the creature index is negative then all creatures in the group are processed */
		creatureIndex = L0329_ps_Group->getCount();

	do {
		AL0326_ui_Aspect = activeGroup->_aspect[creatureIndex];
		AL0326_ui_Aspect &= k0x0080_MaskActiveGroupIsAttacking | k0x0040_MaskActiveGroupFlipBitmap;
		L0328_i_Offset = ((L0327_ui_CreatureGraphicInfo >> 12) & 0x3);
		if (L0328_i_Offset) {
			L0328_i_Offset = _vm->getRandomNumber(L0328_i_Offset);
			if (_vm->getRandomNumber(2))
				L0328_i_Offset = (-L0328_i_Offset) & 0x0007;

			AL0326_ui_Aspect |= L0328_i_Offset;
		}

		L0328_i_Offset = ((L0327_ui_CreatureGraphicInfo >> 14) & 0x3);
		if (L0328_i_Offset) {
			L0328_i_Offset = _vm->getRandomNumber(L0328_i_Offset);
			if (_vm->getRandomNumber(2))
				L0328_i_Offset = (-L0328_i_Offset) & 0x0007;

			AL0326_ui_Aspect |= (L0328_i_Offset << 3);
		}
		if (isAttacking) {
			if (getFlag(L0327_ui_CreatureGraphicInfo, k0x0200_CreatureInfoGraphicMaskFlipAttack)) {
				if (getFlag(AL0326_ui_Aspect, k0x0080_MaskActiveGroupIsAttacking) && (L0331_ui_CreatureType == k18_CreatureTypeAnimatedArmourDethKnight)) {
					if (_vm->getRandomNumber(2)) {
						toggleFlag(AL0326_ui_Aspect, k0x0040_MaskActiveGroupFlipBitmap);
						_vm->_sound->requestPlay(k16_soundCOMBAT_ATTACK_SKELETON_ANIMATED_ARMOUR_DETH_KNIGHT, _currentGroupMapX, _currentGroupMapY, k1_soundModePlayIfPrioritized);
					}
				} else if (!getFlag(AL0326_ui_Aspect, k0x0080_MaskActiveGroupIsAttacking) || !getFlag(L0327_ui_CreatureGraphicInfo, k0x0400_CreatureInfoGraphicMaskFlipDuringAttack)) {
					if (_vm->getRandomNumber(2))
						setFlag(AL0326_ui_Aspect, k0x0040_MaskActiveGroupFlipBitmap);
					else
						clearFlag(AL0326_ui_Aspect, k0x0040_MaskActiveGroupFlipBitmap);
				}
			} else
				clearFlag(AL0326_ui_Aspect, k0x0040_MaskActiveGroupFlipBitmap);

			setFlag(AL0326_ui_Aspect, k0x0080_MaskActiveGroupIsAttacking);
		} else {
			if (getFlag(L0327_ui_CreatureGraphicInfo, k0x0004_CreatureInfoGraphicMaskFlipNonAttack)) {
				if (L0331_ui_CreatureType == k13_CreatureTypeCouatl) {
					if (_vm->getRandomNumber(2)) {
						toggleFlag(AL0326_ui_Aspect, k0x0040_MaskActiveGroupFlipBitmap);
						L1635_ui_SoundIndex = _vm->_moveSens->getSound(k13_CreatureTypeCouatl);
						if (L1635_ui_SoundIndex <= k34_D13_soundCount)
							_vm->_sound->requestPlay(L1635_ui_SoundIndex, _currentGroupMapX, _currentGroupMapY, k1_soundModePlayIfPrioritized);
					}
				} else if (_vm->getRandomNumber(2))
					setFlag(AL0326_ui_Aspect, k0x0040_MaskActiveGroupFlipBitmap);
				else
					clearFlag(AL0326_ui_Aspect, k0x0040_MaskActiveGroupFlipBitmap);
			} else
				clearFlag(AL0326_ui_Aspect, k0x0040_MaskActiveGroupFlipBitmap);

			clearFlag(AL0326_ui_Aspect, k0x0080_MaskActiveGroupIsAttacking);
		}
		activeGroup->_aspect[creatureIndex] = AL0326_ui_Aspect;
	} while (L0330_B_ProcessGroup && (creatureIndex--));
	AL0326_ui_AnimationTicks = _vm->_dungeonMan->_creatureInfos[L0329_ps_Group->_type]._animationTicks;
	return _vm->_gameTime + (isAttacking ? ((AL0326_ui_AnimationTicks >> 8) & 0xF) : ((AL0326_ui_AnimationTicks >> 4) & 0xF)) + _vm->getRandomNumber(2);
}

void GroupMan::setGroupDirection(ActiveGroup *activeGroup, int16 dir, int16 creatureIndex, bool twoHalfSquareSizedCreatures) {
	uint16 L0435_ui_GroupDirections;
	static ActiveGroup *G0396_ps_TwoHalfSquareSizedCreaturesGroupLastDirectionSetActiveGroup;

	if (twoHalfSquareSizedCreatures && (_vm->_gameTime == twoHalfSquareSizedCreaturesGroupLastDirectionSetTime) && (activeGroup == G0396_ps_TwoHalfSquareSizedCreaturesGroupLastDirectionSetActiveGroup)) {
		return;
	}
	L0435_ui_GroupDirections = activeGroup->_directions;
	if (normalizeModulo4(getCreatureValue(L0435_ui_GroupDirections, creatureIndex) - dir) == 2) { /* If current and new direction are opposites then change direction only one step at a time */
		dir = returnNextVal((_vm->getRandomNumber(65536) & 0x0002) + dir);
		L0435_ui_GroupDirections = getGroupValueUpdatedWithCreatureValue(L0435_ui_GroupDirections, creatureIndex, dir);
	} else {
		L0435_ui_GroupDirections = getGroupValueUpdatedWithCreatureValue(L0435_ui_GroupDirections, creatureIndex, dir);
	}
	if (twoHalfSquareSizedCreatures) {
		L0435_ui_GroupDirections = getGroupValueUpdatedWithCreatureValue(L0435_ui_GroupDirections, creatureIndex ^ 1, dir); /* Set direction of the second half square sized creature */
		twoHalfSquareSizedCreaturesGroupLastDirectionSetTime = _vm->_gameTime;
		G0396_ps_TwoHalfSquareSizedCreaturesGroupLastDirectionSetActiveGroup = activeGroup;
	}
	activeGroup->_directions = (Direction)L0435_ui_GroupDirections;
}

void GroupMan::addGroupEvent(TimelineEvent *event, uint32 time) {
	warning("potentially dangerous cast to uint32 below");
	if (time < (uint32)filterTime(event->_mapTime)) {
		event->_type -= 5;
		event->_C._ticks = filterTime(event->_mapTime) - time;
		M32_setTime(event->_mapTime, time);
	} else {
		event->_C._ticks = time - filterTime(event->_mapTime);
	}
	_vm->_timeline->addEventGetEventIndex(event);
}

int16 GroupMan::getSmelledPartyPrimaryDirOrdinal(CreatureInfo *creatureInfo, int16 mapY, int16 mapX) {

	uint16 L0426_ui_SmellRange;
	int16 L0427_i_ScentOrdinal;


	if (!(L0426_ui_SmellRange = creatureInfo->getSmellRange())) {
		return 0;
	}
	if ((((L0426_ui_SmellRange + 1) >> 1) >= _currGroupDistanceToParty) && getDistanceBetweenUnblockedSquares(mapY, mapX, _vm->_dungeonMan->_partyMapX, _vm->_dungeonMan->_partyMapY, &GroupMan::isSmellPartyBlocked)) {
		_vm->_projexpl->_secondaryDirToOrFromParty = _currGroupSecondaryDirToParty;
		return _vm->indexToOrdinal(_currGroupPrimaryDirToParty);
	}
	if ((L0427_i_ScentOrdinal = _vm->_championMan->getScentOrdinal(mapY, mapX)) && ((_vm->_championMan->_party._scentStrengths[_vm->ordinalToIndex(L0427_i_ScentOrdinal)] + _vm->getRandomNumber(4)) > (30 - (L0426_ui_SmellRange << 1)))) { /* If there is a fresh enough party scent on the group square */
		return _vm->indexToOrdinal(getDirsWhereDestIsVisibleFromSource(mapY, mapX, _vm->_championMan->_party._scents[L0427_i_ScentOrdinal].getMapX(), _vm->_championMan->_party._scents[L0427_i_ScentOrdinal].getMapY()));
	}
	return 0;
}

bool GroupMan::isSmellPartyBlocked(uint16 mapX, uint16 mapY) {
	uint16 L0408_ui_Square;
	int16 L0409_i_SquareType;

	return ((L0409_i_SquareType = Square(L0408_ui_Square = _vm->_dungeonMan->_currMapData[mapX][mapY]).getType()) == k0_ElementTypeWall) || ((L0409_i_SquareType == k6_ElementTypeFakeWall) && !getFlag(L0408_ui_Square, k0x0004_FakeWallOpen));
}

int16 GroupMan::getFirstPossibleMovementDirOrdinal(CreatureInfo *info, int16 mapX, int16 mapY, bool allowMovementOverImaginaryPitsAndFakeWalls) {
	int16 L0434_i_Direction;


	for (L0434_i_Direction = kDirNorth; L0434_i_Direction <= kDirWest; L0434_i_Direction++) {
		if ((!_groupMovementTestedDirections[L0434_i_Direction]) && isMovementPossible(info, mapX, mapY, L0434_i_Direction, allowMovementOverImaginaryPitsAndFakeWalls)) {
			return _vm->indexToOrdinal(L0434_i_Direction);
		}
	}
	return 0;
}

void GroupMan::setDirGroup(ActiveGroup *activeGroup, int16 dir, int16 creatureIndex, int16 creatureSize) {
	bool L0436_B_TwoHalfSquareSizedCreatures = creatureIndex && (creatureSize == k1_MaskCreatureSizeHalf);

	if (L0436_B_TwoHalfSquareSizedCreatures)
		creatureIndex--;

	do {
		if (!creatureIndex || _vm->getRandomNumber(2))
			setGroupDirection(activeGroup, dir, creatureIndex, L0436_B_TwoHalfSquareSizedCreatures);
	} while (creatureIndex--);
}

void GroupMan::stopAttacking(ActiveGroup *group, int16 mapX, int16 mapY) {
	int16 L0337_i_CreatureIndex;

	for (L0337_i_CreatureIndex = 0; L0337_i_CreatureIndex < 4; clearFlag(group->_aspect[L0337_i_CreatureIndex++], k0x0080_MaskActiveGroupIsAttacking));
	groupDeleteEvents(mapX, mapY);

}

bool GroupMan::isArchenemyDoubleMovementPossible(CreatureInfo *info, int16 mapX, int16 mapY, uint16 dir) {
	if (_fluxCages[dir]) {
		return false;
	}
	mapX += _vm->_dirIntoStepCountEast[dir], mapY += _vm->_dirIntoStepCountNorth[dir];
	return isMovementPossible(info, mapX, mapY, dir, false);
}

bool GroupMan::isCreatureAttacking(Group *group, int16 mapX, int16 mapY, uint16 creatureIndex) {
	static const uint8 G0244_auc_Graphic559_CreatureAttackSounds[11] = { 3, 7, 14, 15, 19, 21, 29, 30, 31, 4, 16 }; /* Atari ST: { 3, 7, 14, 15, 19, 21, 4, 16 } */

	uint16 L0437_ui_Multiple;
#define AL0437_ui_CreatureType L0437_ui_Multiple
#define AL0437_T_Thing         L0437_ui_Multiple
	int16 L0439_i_Multiple;
#define AL0439_i_GroupCells    L0439_i_Multiple
#define AL0439_i_TargetCell    L0439_i_Multiple
#define AL0439_i_ChampionIndex L0439_i_Multiple
	int16 L0440_i_Multiple;
#define AL0440_i_KineticEnergy      L0440_i_Multiple
#define AL0440_i_Counter            L0440_i_Multiple
#define AL0440_i_Damage             L0440_i_Multiple
#define AL0440_i_AttackSoundOrdinal L0440_i_Multiple

	_vm->_projexpl->_lastCreatureAttackTime = _vm->_gameTime;
	ActiveGroup L0443_s_ActiveGroup = _activeGroups[group->getActiveGroupIndex()];
	CreatureInfo *L0441_ps_CreatureInfo = &_vm->_dungeonMan->_creatureInfos[AL0437_ui_CreatureType = group->_type];
	uint16 L0438_ui_PrimaryDirectionToParty = _currGroupPrimaryDirToParty;
	if ((AL0439_i_GroupCells = L0443_s_ActiveGroup._cells) == k255_CreatureTypeSingleCenteredCreature) {
		AL0439_i_TargetCell = _vm->getRandomNumber(2);
	} else {
		AL0439_i_TargetCell = ((getCreatureValue(AL0439_i_GroupCells, creatureIndex) + 5 - L0438_ui_PrimaryDirectionToParty) & 0x0002) >> 1;
	}
	AL0439_i_TargetCell += L0438_ui_PrimaryDirectionToParty;
	AL0439_i_TargetCell &= 0x0003;
	if ((L0441_ps_CreatureInfo->getAttackRange() > 1) && ((_currGroupDistanceToParty > 1) || _vm->getRandomNumber(2))) {
		switch (AL0437_ui_CreatureType) {
		case k14_CreatureTypeVexirk:
		case k23_CreatureTypeLordChaos:
			if (_vm->getRandomNumber(2)) {
				AL0437_T_Thing = Thing::_explFireBall.toUint16();
			} else {
				switch (_vm->getRandomNumber(4)) {
				case 0:
					AL0437_T_Thing = Thing::_explHarmNonMaterial.toUint16();
					break;
				case 1:
					AL0437_T_Thing = Thing::_explLightningBolt.toUint16();
					break;
				case 2:
					AL0437_T_Thing = Thing::_explPoisonCloud.toUint16();
					break;
				case 3:
					AL0437_T_Thing = Thing::_explOpenDoor.toUint16();
				}
			}
			break;
		case k1_CreatureTypeSwampSlimeSlime:
			AL0437_T_Thing = Thing::_explSlime.toUint16();
			break;
		case k3_CreatureTypeWizardEyeFlyingEye:
			if (_vm->getRandomNumber(8)) {
				AL0437_T_Thing = Thing::_explLightningBolt.toUint16();
			} else {
				AL0437_T_Thing = Thing::_explOpenDoor.toUint16();
			}
			break;
		case k19_CreatureTypeMaterializerZytaz:
			if (_vm->getRandomNumber(2)) {
				AL0437_T_Thing = Thing::_explPoisonCloud.toUint16();
				break;
			}
		case k22_CreatureTypeDemon:
		case k24_CreatureTypeRedDragon:
			AL0437_T_Thing = Thing::_explFireBall.toUint16();
		} /* BUG0_13 The game may crash when 'Lord Order' or 'Grey Lord' cast spells. This cannot happen with the original dungeons as they do not contain any groups of these types. 'Lord Order' and 'Grey Lord' creatures can cast spells (attack range > 1) but no projectile type is defined for them in the code. If these creatures are present in a dungeon they will cast projectiles containing undefined things because the variable is not initialized */
		AL0440_i_KineticEnergy = (L0441_ps_CreatureInfo->_attack >> 2) + 1;
		AL0440_i_KineticEnergy += _vm->getRandomNumber(AL0440_i_KineticEnergy);
		AL0440_i_KineticEnergy += _vm->getRandomNumber(AL0440_i_KineticEnergy);
		_vm->_sound->requestPlay(k13_soundSPELL, mapX, mapY, k0_soundModePlayImmediately);
		_vm->_projexpl->createProjectile(Thing(AL0437_T_Thing), mapX, mapY, AL0439_i_TargetCell, (Direction)_currGroupPrimaryDirToParty, getBoundedValue((int16)20, AL0440_i_KineticEnergy, (int16)255), L0441_ps_CreatureInfo->_dexterity, 8);
	} else {
		if (getFlag(L0441_ps_CreatureInfo->_attributes, k0x0010_MaskCreatureInfo_attackAnyChamp)) {
			AL0439_i_ChampionIndex = _vm->getRandomNumber(4);
			for (AL0440_i_Counter = 0; (AL0440_i_Counter < 4) && !_vm->_championMan->_champions[AL0439_i_ChampionIndex]._currHealth; AL0440_i_Counter++) {
				AL0439_i_ChampionIndex = returnNextVal(AL0439_i_ChampionIndex);
			}
			if (AL0440_i_Counter == 4) {
				return false;
			}
		} else {
			if ((AL0439_i_ChampionIndex = _vm->_championMan->getTargetChampionIndex(mapX, mapY, AL0439_i_TargetCell)) < 0) {
				return false;
			}
		}
		if (AL0437_ui_CreatureType == k2_CreatureTypeGiggler) {
			stealFromChampion(group, AL0439_i_ChampionIndex);
		} else {
			AL0440_i_Damage = getChampionDamage(group, AL0439_i_ChampionIndex) + 1;
			Champion *L0442_ps_Champion = &_vm->_championMan->_champions[AL0439_i_ChampionIndex];
			if (AL0440_i_Damage > L0442_ps_Champion->_maximumDamageReceived) {
				L0442_ps_Champion->_maximumDamageReceived = AL0440_i_Damage;
				L0442_ps_Champion->_directionMaximumDamageReceived = returnOppositeDir((Direction)L0438_ui_PrimaryDirectionToParty);
			}
		}
	}
	AL0440_i_AttackSoundOrdinal = L0441_ps_CreatureInfo->_attackSoundOrdinal;
	if (AL0440_i_AttackSoundOrdinal)
		_vm->_sound->requestPlay(G0244_auc_Graphic559_CreatureAttackSounds[--AL0440_i_AttackSoundOrdinal], mapX, mapY, k1_soundModePlayIfPrioritized);

	return true;
}

void GroupMan::setOrderedCellsToAttack(signed char *orderedCellsToAttack, int16 targetMapX, int16 targetMapY, int16 attackerMapX, int16 attackerMapY, uint16 cellSource) {
	static signed char g23_orderedCellsToAttack[8][4] = { // @ G0023_aac_Graphic562_OrderedCellsToAttack
		{0, 1, 3, 2},   /* Attack South from position Northwest or Southwest */
		{1, 0, 2, 3},   /* Attack South from position Northeast or Southeast */
		{1, 2, 0, 3},   /* Attack West from position Northwest or Northeast */
		{2, 1, 3, 0},   /* Attack West from position Southeast or Southwest */
		{3, 2, 0, 1},   /* Attack North from position Northwest or Southwest */
		{2, 3, 1, 0},   /* Attack North from position Southeast or Northeast */
		{0, 3, 1, 2},   /* Attack East from position Northwest or Northeast */
		{3, 0, 2, 1}}; /* Attack East from position Southeast or Southwest */
	uint16 L0557_ui_OrderedCellsToAttackIndex;


	if (!((L0557_ui_OrderedCellsToAttackIndex = getDirsWhereDestIsVisibleFromSource(targetMapX, targetMapY, attackerMapX, attackerMapY) << 1) & 0x0002)) {
		cellSource++;
	}
	L0557_ui_OrderedCellsToAttackIndex += (cellSource >> 1) & 0x0001;
	for (uint16 i = 0; i < 4; ++i)
		orderedCellsToAttack[i] = g23_orderedCellsToAttack[L0557_ui_OrderedCellsToAttackIndex][i];
}

void GroupMan::stealFromChampion(Group *group, uint16 championIndex) {
	int16 L0391_i_Percentage;
	uint16 L0392_ui_StealFromSlotIndex;
	uint16 L0393_ui_Counter;
	Thing L0394_T_Thing;
	Champion *L0395_ps_Champion;
	bool L0396_B_ObjectStolen;
	static unsigned char G0394_auc_StealFromSlotIndices[8]; /* Initialized with 0 bytes by C loader */


	L0396_B_ObjectStolen = false;
	L0391_i_Percentage = 100 - _vm->_championMan->getDexterity(L0395_ps_Champion = &_vm->_championMan->_champions[championIndex]);
	L0393_ui_Counter = _vm->getRandomNumber(8);
	while ((L0391_i_Percentage > 0) && !_vm->_championMan->isLucky(L0395_ps_Champion, L0391_i_Percentage)) {
		if ((L0392_ui_StealFromSlotIndex = G0394_auc_StealFromSlotIndices[L0393_ui_Counter]) == k13_ChampionSlotBackpackLine_1_1) {
			L0392_ui_StealFromSlotIndex += _vm->getRandomNumber(17); /* Select a random slot in the backpack */
		}
		if (((L0394_T_Thing = L0395_ps_Champion->_slots[L0392_ui_StealFromSlotIndex]) != Thing::_none)) {
			L0396_B_ObjectStolen = true;
			L0394_T_Thing = _vm->_championMan->getObjectRemovedFromSlot(championIndex, L0392_ui_StealFromSlotIndex);
			if (group->_slot == Thing::_endOfList) {
				group->_slot = L0394_T_Thing; /* BUG0_12 An object is cloned and appears at two different locations in the dungeon and/or inventory. The game may crash when interacting with this object. If a Giggler with no possessions steals an object that was previously in a chest and was not the last object in the chest then the objects that followed it are cloned. In the chest, the object is part of a linked list of objects that is not reset when the object is removed from the chest and placed in the inventory (but not in the dungeon), nor when it is stolen and added as the first Giggler possession. If the Giggler already has a possession before stealing the object then this does not create a cloned object.
											 The following statement is missing: L0394_T_Thing->Next = Thing::_endOfList;
											 This creates cloned things if L0394_T_Thing->Next is not Thing::_endOfList which is the case when the object comes from a chest in which it was not the last object */
			} else {
				_vm->_dungeonMan->linkThingToList(L0394_T_Thing, group->_slot, kM1_MapXNotOnASquare, 0);
			}
			_vm->_championMan->drawChampionState((ChampionIndex)championIndex);
		}
		++L0393_ui_Counter;
		L0393_ui_Counter &= 0x0007;
		L0391_i_Percentage -= 20;
	}
	if (!_vm->getRandomNumber(8) || (L0396_B_ObjectStolen && _vm->getRandomNumber(2))) {
		_activeGroups[group->getActiveGroupIndex()]._delayFleeingFromTarget = _vm->getRandomNumber(64) + 20;
		group->setBehaviour(k5_behavior_FLEE);
	}
}

int16 GroupMan::getChampionDamage(Group *group, uint16 champIndex) {
	unsigned char g24_woundProbabilityIndexToWoundMask[4] = {32, 16, 8, 4}; // @ G0024_auc_Graphic562_WoundProbabilityIndexToWoundMask

	Champion *L0562_ps_Champion;
	int16 L0558_i_Multiple;
#define AL0558_i_Attack L0558_i_Multiple
#define AL0558_i_Damage L0558_i_Multiple
	uint16 L0559_ui_Multiple;
#define AL0559_ui_WoundTest          L0559_ui_Multiple
#define AL0559_ui_PoisonAttack       L0559_ui_Multiple
#define AL0559_ui_CreatureDifficulty L0559_ui_Multiple
	uint16 L0560_ui_WoundProbabilities;
	uint16 L0561_ui_Multiple;
#define AL0561_ui_WoundProbabilityIndex L0561_ui_Multiple
#define AL0561_ui_AllowedWound          L0561_ui_Multiple
	int16 L0563_i_DoubledMapDifficulty;
	CreatureInfo L0564_s_CreatureInfo;


	L0562_ps_Champion = &_vm->_championMan->_champions[champIndex];
	if (champIndex >= _vm->_championMan->_partyChampionCount) {
		return 0;
	}
	if (!L0562_ps_Champion->_currHealth) {
		return 0;
	}
	if (_vm->_championMan->_partyIsSleeping) {
		_vm->_championMan->wakeUp();
	}
	L0563_i_DoubledMapDifficulty = _vm->_dungeonMan->_currMap->_difficulty << 1;
	L0564_s_CreatureInfo = _vm->_dungeonMan->_creatureInfos[group->_type];
	_vm->_championMan->addSkillExperience(champIndex, k7_ChampionSkillParry, L0564_s_CreatureInfo.getExperience());
	if (_vm->_championMan->_partyIsSleeping || (((_vm->_championMan->getDexterity(L0562_ps_Champion) < (_vm->getRandomNumber(32) + L0564_s_CreatureInfo._dexterity + L0563_i_DoubledMapDifficulty - 16)) || !_vm->getRandomNumber(4)) && !_vm->_championMan->isLucky(L0562_ps_Champion, 60))) {
		if ((AL0559_ui_WoundTest = _vm->getRandomNumber(65536)) & 0x0070) {
			AL0559_ui_WoundTest &= 0x000F;
			L0560_ui_WoundProbabilities = L0564_s_CreatureInfo._woundProbabilities;
			for (AL0561_ui_WoundProbabilityIndex = 0; AL0559_ui_WoundTest > (L0560_ui_WoundProbabilities & 0x000F); L0560_ui_WoundProbabilities >>= 4) {
				AL0561_ui_WoundProbabilityIndex++;
			}
			AL0561_ui_AllowedWound = g24_woundProbabilityIndexToWoundMask[AL0561_ui_WoundProbabilityIndex];
		} else {
			AL0561_ui_AllowedWound = AL0559_ui_WoundTest & 0x0001; /* 0 (Ready hand) or 1 (action hand) */
		}
		if ((AL0558_i_Attack = (_vm->getRandomNumber(16) + L0564_s_CreatureInfo._attack + L0563_i_DoubledMapDifficulty) - (_vm->_championMan->getSkillLevel(champIndex, k7_ChampionSkillParry) << 1)) <= 1) {
			if (_vm->getRandomNumber(2)) {
				goto T0230014;
			}
			AL0558_i_Attack = _vm->getRandomNumber(4) + 2;
		}
		AL0558_i_Attack >>= 1;
		AL0558_i_Attack += _vm->getRandomNumber(AL0558_i_Attack) + _vm->getRandomNumber(4);
		AL0558_i_Attack += _vm->getRandomNumber(AL0558_i_Attack);
		AL0558_i_Attack >>= 2;
		AL0558_i_Attack += _vm->getRandomNumber(4) + 1;
		if (_vm->getRandomNumber(2))
			AL0558_i_Attack -= _vm->getRandomNumber((AL0558_i_Attack >> 1) + 1) - 1;

		AL0558_i_Damage = _vm->_championMan->addPendingDamageAndWounds_getDamage(champIndex, AL0558_i_Attack, AL0561_ui_AllowedWound, L0564_s_CreatureInfo._attackType);
		if (AL0558_i_Damage) {
			_vm->_sound->requestPlay(k09_soundCHAMPION_0_DAMAGED + champIndex, _vm->_dungeonMan->_partyMapX, _vm->_dungeonMan->_partyMapY, k2_soundModePlayOneTickLater);

			AL0559_ui_PoisonAttack = L0564_s_CreatureInfo._poisonAttack;
			if (AL0559_ui_PoisonAttack && _vm->getRandomNumber(2)) {
				AL0559_ui_PoisonAttack = _vm->_championMan->getStatisticAdjustedAttack(L0562_ps_Champion, k4_ChampionStatVitality, AL0559_ui_PoisonAttack);
				if (AL0559_ui_PoisonAttack >= 0)
					_vm->_championMan->championPoison(champIndex, AL0559_ui_PoisonAttack);
			}
			return AL0558_i_Damage;
		}
	}
T0230014:
	return 0;
}

void GroupMan::dropMovingCreatureFixedPossession(Thing thing, int16 mapX, int16 mapY) {
	Group *L0363_ps_Group;
	int16 L0364_i_CreatureType;


	if (_dropMovingCreatureFixedPossCellCount) {
		L0363_ps_Group = (Group *)_vm->_dungeonMan->getThingData(thing);
		L0364_i_CreatureType = L0363_ps_Group->_type;
		while (_dropMovingCreatureFixedPossCellCount) {
			dropCreatureFixedPossessions(L0364_i_CreatureType, mapX, mapY, _dropMovingCreatureFixedPossessionsCell[--_dropMovingCreatureFixedPossCellCount], k2_soundModePlayOneTickLater);
		}
	}
}

void GroupMan::startWanedring(int16 mapX, int16 mapY) {
	Group *L0332_ps_Group;
	TimelineEvent L0333_s_Event;


	L0332_ps_Group = (Group *)_vm->_dungeonMan->getThingData(groupGetThing(mapX, mapY));
	if (L0332_ps_Group->getBehaviour() >= k4_behavior_USELESS) {
		L0332_ps_Group->setBehaviour(k0_behavior_WANDER);
	}
	setMapAndTime(L0333_s_Event._mapTime, _vm->_dungeonMan->_currMapIndex, (_vm->_gameTime + 1));
	L0333_s_Event._type = k37_TMEventTypeUpdateBehaviourGroup;
	L0333_s_Event._priority = 255 - _vm->_dungeonMan->_creatureInfos[L0332_ps_Group->_type]._movementTicks; /* The fastest creatures (with small MovementTicks value) get higher event priority */
	L0333_s_Event._C._ticks = 0;
	L0333_s_Event._B._location._mapX = mapX;
	L0333_s_Event._B._location._mapY = mapY;
	_vm->_timeline->addEventGetEventIndex(&L0333_s_Event);
}

void GroupMan::addActiveGroup(Thing thing, int16 mapX, int16 mapY) {
	uint16 L0339_ui_CreatureIndex;
	Group *L0340_ps_Group;
	ActiveGroup *L0341_ps_ActiveGroup;
	int16 L0344_i_ActiveGroupIndex;


	L0341_ps_ActiveGroup = _activeGroups;
	L0344_i_ActiveGroupIndex = 0;
	while (L0341_ps_ActiveGroup->_groupThingIndex >= 0) {
		if (++L0344_i_ActiveGroupIndex >= _maxActiveGroupCount) {
			return;
		}
		L0341_ps_ActiveGroup++;
	}
	_currActiveGroupCount++;

	L0340_ps_Group = (Group *)(_vm->_dungeonMan->_thingData[k4_GroupThingType] + 
		_vm->_dungeonMan->_thingDataWordCount[k4_GroupThingType] * (L0341_ps_ActiveGroup->_groupThingIndex = (thing).getIndex()));

	L0341_ps_ActiveGroup->_cells = L0340_ps_Group->_cells;
	L0340_ps_Group->getActiveGroupIndex() = L0344_i_ActiveGroupIndex;
	L0341_ps_ActiveGroup->_priorMapX = L0341_ps_ActiveGroup->_homeMapX = mapX;
	L0341_ps_ActiveGroup->_priorMapY = L0341_ps_ActiveGroup->_homeMapY = mapY;
	L0341_ps_ActiveGroup->_lastMoveTime = _vm->_gameTime - 127;
	L0339_ui_CreatureIndex = L0340_ps_Group->getCount();
	do {
		L0341_ps_ActiveGroup->_directions = (Direction)getGroupValueUpdatedWithCreatureValue(L0341_ps_ActiveGroup->_directions, L0339_ui_CreatureIndex, L0340_ps_Group->getDir());
		L0341_ps_ActiveGroup->_aspect[L0339_ui_CreatureIndex] = 0;
	} while (L0339_ui_CreatureIndex--);
	getCreatureAspectUpdateTime(L0341_ps_ActiveGroup, kM1_wholeCreatureGroup, false);
}

void GroupMan::removeActiveGroup(uint16 activeGroupIndex) {
	ActiveGroup *L0347_ps_ActiveGroup;
	Group *L0348_ps_Group;


	if ((activeGroupIndex > _maxActiveGroupCount) || (_activeGroups[activeGroupIndex]._groupThingIndex < 0)) {
		return;
	}
	L0347_ps_ActiveGroup = &_activeGroups[activeGroupIndex];
	L0348_ps_Group = &((Group *)_vm->_dungeonMan->_thingData[k4_GroupThingType])[L0347_ps_ActiveGroup->_groupThingIndex];
	_currActiveGroupCount--;
	L0348_ps_Group->_cells = L0347_ps_ActiveGroup->_cells;
	L0348_ps_Group->setDir(normalizeModulo4(L0347_ps_ActiveGroup->_directions));
	if (L0348_ps_Group->getBehaviour() >= k4_behavior_USELESS) {
		L0348_ps_Group->setBehaviour(k0_behavior_WANDER);
	}
	L0347_ps_ActiveGroup->_groupThingIndex = -1;
}

void GroupMan::removeAllActiveGroups() {
	for (int16 L0397_ui_ActiveGroupIndex = 0; _currActiveGroupCount > 0; L0397_ui_ActiveGroupIndex++) {
		if (_activeGroups[L0397_ui_ActiveGroupIndex]._groupThingIndex >= 0) {
			removeActiveGroup(L0397_ui_ActiveGroupIndex);
		}
	}
}

void GroupMan::addAllActiveGroups() {
	uint16 L0398_ui_MapX;
	uint16 L0399_ui_MapY;
	Thing L0400_T_Thing;
	byte *L0401_puc_Square;
	Thing *L0402_pT_SquareFirstThing;


	L0401_puc_Square = _vm->_dungeonMan->_currMapData[0];
	L0402_pT_SquareFirstThing = &_vm->_dungeonMan->_squareFirstThings[_vm->_dungeonMan->_currMapColCumulativeSquareFirstThingCount[0]];
	for (L0398_ui_MapX = 0; L0398_ui_MapX < _vm->_dungeonMan->_currMapWidth; L0398_ui_MapX++) {
		for (L0399_ui_MapY = 0; L0399_ui_MapY < _vm->_dungeonMan->_currMapHeight; L0399_ui_MapY++) {
			if (getFlag(*L0401_puc_Square++, k0x0010_ThingListPresent)) {
				L0400_T_Thing = *L0402_pT_SquareFirstThing++;
				do {
					if (L0400_T_Thing.getType() == k4_GroupThingType) {
						groupDeleteEvents(L0398_ui_MapX, L0399_ui_MapY);
						addActiveGroup(L0400_T_Thing, L0398_ui_MapX, L0399_ui_MapY);
						startWanedring(L0398_ui_MapX, L0399_ui_MapY);
						break;
					}
				} while ((L0400_T_Thing = _vm->_dungeonMan->getNextThing(L0400_T_Thing)) != Thing::_endOfList);
			}
		}
	}
}

Thing GroupMan::groupGetGenerated(int16 creatureType, int16 healthMultiplier, uint16 creatureCount, Direction dir, int16 mapX, int16 mapY) {
	Thing L0349_T_GroupThing;
	uint16 L0350_ui_BaseHealth;
	uint16 L0351_ui_Cell = 0;
	uint16 L0352_ui_GroupCells = 0;
	Group* L0353_ps_Group;
	CreatureInfo* L0354_ps_CreatureInfo;
	bool L0355_B_SeveralCreaturesInGroup;


	if (((_currActiveGroupCount >= (_maxActiveGroupCount - 5)) && (_vm->_dungeonMan->_currMapIndex == _vm->_dungeonMan->_partyMapIndex)) || ((L0349_T_GroupThing = _vm->_dungeonMan->getUnusedThing(k4_GroupThingType)) == Thing::_none)) {
		return Thing::_none;
	}
	L0353_ps_Group = (Group*)_vm->_dungeonMan->getThingData(L0349_T_GroupThing);
	L0353_ps_Group->_slot = Thing::_endOfList;
	L0353_ps_Group->setDoNotDiscard(false);
	L0353_ps_Group->setDir(dir);
	L0353_ps_Group->setCount(creatureCount);
	L0355_B_SeveralCreaturesInGroup = creatureCount;
	if (L0355_B_SeveralCreaturesInGroup)
		L0351_ui_Cell = _vm->getRandomNumber(4);
	else
		L0352_ui_GroupCells = k255_CreatureTypeSingleCenteredCreature;

	L0354_ps_CreatureInfo = &_vm->_dungeonMan->_creatureInfos[L0353_ps_Group->_type = creatureType];
	L0350_ui_BaseHealth = L0354_ps_CreatureInfo->_baseHealth;
	do {
		L0353_ps_Group->_health[creatureCount] = (L0350_ui_BaseHealth * healthMultiplier) + _vm->getRandomNumber((L0350_ui_BaseHealth >> 2) + 1);
		if (L0355_B_SeveralCreaturesInGroup) {
			L0352_ui_GroupCells = getGroupValueUpdatedWithCreatureValue(L0352_ui_GroupCells, creatureCount, L0351_ui_Cell++);
			if (getFlag(L0354_ps_CreatureInfo->_attributes, k0x0003_MaskCreatureInfo_size) == k1_MaskCreatureSizeHalf) {
				L0351_ui_Cell++;
			}
			L0351_ui_Cell &= 0x0003;
		}
	} while (creatureCount--);
	L0353_ps_Group->_cells = L0352_ui_GroupCells;
	if (_vm->_moveSens->getMoveResult(L0349_T_GroupThing, kM1_MapXNotOnASquare, 0, mapX, mapY)) { /* If F0267_MOVE_GetMoveResult_CPSCE returns true then the group was either killed by a projectile impact (in which case the thing data was marked as unused) or the party is on the destination square and an event is created to move the creature into the dungeon later (in which case the thing is referenced in the event) */
		return Thing::_none;
	}
	_vm->_sound->requestPlay(k17_soundBUZZ, mapX, mapY, k1_soundModePlayIfPrioritized);
	return L0349_T_GroupThing;
}

bool GroupMan::isSquareACorridorTeleporterPitOrDoor(int16 mapX, int16 mapY) {
	int16 L0544_i_SquareType;

	return (((L0544_i_SquareType = Square(_vm->_dungeonMan->getSquare(mapX, mapY)).getType()) == k1_CorridorElemType)
			|| (L0544_i_SquareType == k5_ElementTypeTeleporter) || (L0544_i_SquareType == k2_ElementTypePit) || (L0544_i_SquareType == k4_DoorElemType));
}

int16 GroupMan::getMeleeTargetCreatureOrdinal(int16 groupX, int16 groupY, int16 partyX, int16 partyY, uint16 champCell) {
	uint16 L0321_ui_Counter;
	int16 L0322_i_CreatureOrdinal;
	Thing L0323_T_GroupThing;
	Group* L0324_ps_Group;
	signed char L0325_auc_OrderedCellsToAttack[4];

	L0323_T_GroupThing = groupGetThing(groupX, groupY);
	if (L0323_T_GroupThing == Thing::_endOfList)
		return 0;

	L0324_ps_Group = (Group*)_vm->_dungeonMan->getThingData(L0323_T_GroupThing);
	setOrderedCellsToAttack(L0325_auc_OrderedCellsToAttack, groupX, groupY, partyX, partyY, champCell);
	L0321_ui_Counter = 0;
	for (;;) { /*_Infinite loop_*/
		L0322_i_CreatureOrdinal = getCreatureOrdinalInCell(L0324_ps_Group, L0325_auc_OrderedCellsToAttack[L0321_ui_Counter]);
		if (L0322_i_CreatureOrdinal)
			return L0322_i_CreatureOrdinal;

		L0321_ui_Counter++;
	}
}

int16 GroupMan::getMeleeActionDamage(Champion* champ, int16 champIndex, Group* group, int16 creatureIndex, int16 mapX, int16 mapY, uint16 actionHitProbability, uint16 actionDamageFactor, int16 skillIndex) {
	int16 L0565_i_Damage = 0;
	int16 L0566_i_Damage = 0;
	int16 L0567_i_DoubledMapDifficulty;
	int16 L0568_i_Defense;
	int16 L0569_i_Outcome;
	bool L0570_B_ActionHitsNonMaterialCreatures;
	int16 L0571_i_ActionHandObjectIconIndex;
	CreatureInfo* L0572_ps_CreatureInfo;

	if (champIndex >= _vm->_championMan->_partyChampionCount) {
		return 0;
	}
	if (!champ->_currHealth) {
		return 0;
	}
	L0567_i_DoubledMapDifficulty = _vm->_dungeonMan->_currMap->_difficulty << 1;
	L0572_ps_CreatureInfo = &_vm->_dungeonMan->_creatureInfos[group->_type];
	L0571_i_ActionHandObjectIconIndex = _vm->_objectMan->getIconIndex(champ->_slots[k1_ChampionSlotActionHand]);
	L0570_B_ActionHitsNonMaterialCreatures = getFlag(actionHitProbability, k0x8000_hitNonMaterialCreatures);
	if (L0570_B_ActionHitsNonMaterialCreatures)
		clearFlag(actionHitProbability, k0x8000_hitNonMaterialCreatures);

	if ((!getFlag(L0572_ps_CreatureInfo->_attributes, k0x0040_MaskCreatureInfo_nonMaterial) || L0570_B_ActionHitsNonMaterialCreatures) &&
		((_vm->_championMan->getDexterity(champ) > (_vm->getRandomNumber(32) + L0572_ps_CreatureInfo->_dexterity + L0567_i_DoubledMapDifficulty - 16)) ||
		(!_vm->getRandomNumber(4)) ||
		 (_vm->_championMan->isLucky(champ, 75 - actionHitProbability)))) {

		L0565_i_Damage = _vm->_championMan->getStrength(champIndex, k1_ChampionSlotActionHand);
		if (!(L0565_i_Damage))
			goto T0231009;

		L0565_i_Damage += _vm->getRandomNumber((L0565_i_Damage >> 1) + 1);
		L0565_i_Damage = ((long)L0565_i_Damage * (long)actionDamageFactor) >> 5;
		L0568_i_Defense = _vm->getRandomNumber(32) + L0572_ps_CreatureInfo->_defense + L0567_i_DoubledMapDifficulty;
		if (L0571_i_ActionHandObjectIconIndex == k39_IconIndiceWeaponDiamondEdge)
			L0568_i_Defense -= L0568_i_Defense >> 2;
		else if (L0571_i_ActionHandObjectIconIndex == k43_IconIndiceWeaponHardcleaveExecutioner)
			L0568_i_Defense -= L0568_i_Defense >> 3;

		L0565_i_Damage += _vm->getRandomNumber(32) - L0568_i_Defense;
		L0566_i_Damage = L0565_i_Damage;
		if (L0566_i_Damage <= 1) {
T0231009:
			L0565_i_Damage = _vm->getRandomNumber(4);
			if (!L0565_i_Damage)
				goto T0231015;

			L0565_i_Damage++;
			if (((L0566_i_Damage += _vm->getRandomNumber(16)) > 0) || (_vm->getRandomNumber(2))) {
				L0565_i_Damage += _vm->getRandomNumber(4);
				if (!_vm->getRandomNumber(4)) {
					L0565_i_Damage += MAX(0, L0566_i_Damage + _vm->getRandomNumber(16));
				}
			}
		}
		L0565_i_Damage >>= 1;
		L0565_i_Damage += _vm->getRandomNumber(L0565_i_Damage) + _vm->getRandomNumber(4);
		L0565_i_Damage += _vm->getRandomNumber(L0565_i_Damage);
		L0565_i_Damage >>= 2;
		L0565_i_Damage += _vm->getRandomNumber(4) + 1;
		if ((L0571_i_ActionHandObjectIconIndex == k40_IconIndiceWeaponVorpalBlade) && !getFlag(L0572_ps_CreatureInfo->_attributes, k0x0040_MaskCreatureInfo_nonMaterial) && !(L0565_i_Damage >>= 1))
			goto T0231015;
		if (_vm->getRandomNumber(64) < _vm->_championMan->getSkillLevel(champIndex, skillIndex)) {
			L0565_i_Damage += L0565_i_Damage + 10;
		}
		L0569_i_Outcome = groupGetDamageCreatureOutcome(group, creatureIndex, mapX, mapY, L0565_i_Damage, true);
		_vm->_championMan->addSkillExperience(champIndex, skillIndex, (L0565_i_Damage * L0572_ps_CreatureInfo->getExperience() >> 4) + 3);
		_vm->_championMan->decrementStamina(champIndex, _vm->getRandomNumber(4) + 4);
		goto T0231016;
	}
T0231015:
	L0565_i_Damage = 0;
	L0569_i_Outcome = k0_outcomeKilledNoCreaturesInGroup;
	_vm->_championMan->decrementStamina(champIndex, _vm->getRandomNumber(2) + 2);
T0231016:
	_vm->_championMan->drawChampionState((ChampionIndex)champIndex);
	if (L0569_i_Outcome != k2_outcomeKilledAllCreaturesInGroup) {
		processEvents29to41(mapX, mapY, kM1_TMEventTypeCreateReactionEvent31ParyIsAdjacent, 0);
	}
	return L0565_i_Damage;
}

void GroupMan::fluxCageAction(int16 mapX, int16 mapY) {
	Thing L0545_T_Thing;
	int16 L0546_i_Multiple;
#define AL0546_i_SquareType    L0546_i_Multiple
#define AL0546_i_FluxcageCount L0546_i_Multiple
	TimelineEvent L0547_s_Event;


	AL0546_i_SquareType = _vm->_dungeonMan->getSquare(mapX, mapY).getType();
	if ((AL0546_i_SquareType == k0_ElementTypeWall) || (AL0546_i_SquareType == k3_ElementTypeStairs))
		return;

	L0545_T_Thing = _vm->_dungeonMan->getUnusedThing(k15_ExplosionThingType);
	if (L0545_T_Thing == Thing::_none)
		return;

	_vm->_dungeonMan->linkThingToList(L0545_T_Thing, Thing(0), mapX, mapY);
	(((Explosion*)_vm->_dungeonMan->_thingData[k15_ExplosionThingType])[L0545_T_Thing.getIndex()]).setType(k50_ExplosionType_Fluxcage);
	setMapAndTime(L0547_s_Event._mapTime, _vm->_dungeonMan->_currMapIndex, _vm->_gameTime + 100);
	L0547_s_Event._type = k24_TMEventTypeRemoveFluxcage;
	L0547_s_Event._priority = 0;
	L0547_s_Event._C._slot = L0545_T_Thing.toUint16();
	L0547_s_Event._B._location._mapX = mapX;
	L0547_s_Event._B._location._mapY = mapY;
	L0547_s_Event._B._location._mapY = mapY;
	_vm->_timeline->addEventGetEventIndex(&L0547_s_Event);
	if (isLordChaosOnSquare(mapX, mapY - 1)) {
		mapY--;
		AL0546_i_FluxcageCount = isFluxcageOnSquare(mapX + 1, mapY);
		goto T0224005;
	}
	if (isLordChaosOnSquare(mapX - 1, mapY)) {
		mapX--;
		AL0546_i_FluxcageCount = isFluxcageOnSquare(mapX, mapY + 1);
T0224005:
		AL0546_i_FluxcageCount += isFluxcageOnSquare(mapX, mapY - 1) + isFluxcageOnSquare(mapX - 1, mapY);
	} else {
		if (isLordChaosOnSquare(mapX + 1, mapY)) {
			mapX++;
			AL0546_i_FluxcageCount = isFluxcageOnSquare(mapX, mapY - 1);
			goto T0224008;
		}
		if (isLordChaosOnSquare(mapX, mapY + 1)) {
			mapY++;
			AL0546_i_FluxcageCount = isFluxcageOnSquare(mapX - 1, mapY);
T0224008:
			AL0546_i_FluxcageCount += isFluxcageOnSquare(mapX, mapY + 1) + isFluxcageOnSquare(mapX + 1, mapY);
		} else {
			AL0546_i_FluxcageCount = 0;
		}
	}
	if (AL0546_i_FluxcageCount == 2) {
		processEvents29to41(mapX, mapY, kM3_TMEventTypeCreateReactionEvent29DangerOnSquare, 0);
	}
}

uint16 GroupMan::isLordChaosOnSquare(int16 mapX, int16 mapY) {
	Thing L0542_T_Thing;
	Group* L0543_ps_Group;

	L0542_T_Thing = groupGetThing(mapX, mapY);
	if (L0542_T_Thing == Thing::_endOfList)
		return 0;

	L0543_ps_Group = (Group*)_vm->_dungeonMan->getThingData(L0542_T_Thing);
	if (L0543_ps_Group->_type == k23_CreatureTypeLordChaos)
		return L0542_T_Thing.toUint16();

	return 0;
}

bool GroupMan::isFluxcageOnSquare(int16 mapX, int16 mapY) {
	Thing L0540_T_Thing;
	int16 L0541_i_SquareType;

	L0541_i_SquareType = _vm->_dungeonMan->getSquare(mapX, mapY).getType();
	if ((L0541_i_SquareType == k0_ElementTypeWall) || (L0541_i_SquareType == k3_ElementTypeStairs))
		return false;

	L0540_T_Thing = _vm->_dungeonMan->getSquareFirstThing(mapX, mapY);
	while (L0540_T_Thing != Thing::_endOfList) {
		if ((L0540_T_Thing.getType() == k15_ExplosionThingType) && (((Explosion*)_vm->_dungeonMan->_thingData[k15_ExplosionThingType])[L0540_T_Thing.getIndex()].getType() == k50_ExplosionType_Fluxcage)) {
			return true;
		}
		L0540_T_Thing = _vm->_dungeonMan->getNextThing(L0540_T_Thing);
	}
	return false;
}

void GroupMan::fuseAction(uint16 mapX, uint16 mapY) {
	int16 L0548_i_MapX;
	int16 L0549_i_MapY;
	uint16 L0551_ui_FluxcageCount;
	uint16 L0552_ui_FluxcageIndex;
	uint16 L0553_ui_Counter;
	bool L0554_aB_Fluxcages[4];
	Thing L0555_T_LordChaosThing;

	if ((mapX < 0) || (mapX >= _vm->_dungeonMan->_currMapWidth) || (mapY < 0) || (mapY >= _vm->_dungeonMan->_currMapHeight)) {
		return;
	}

	_vm->_projexpl->createExplosion(Thing::_explHarmNonMaterial, 255, mapX, mapY, k255_CreatureTypeSingleCenteredCreature); /* BUG0_17 The game crashes after the Fuse action is performed while looking at a wall on a map boundary. An explosion thing is created on the square in front of the party but there is no check to ensure the square coordinates are in the map bounds. This corrupts a memory location and leads to a game crash */
	L0555_T_LordChaosThing = Thing(isLordChaosOnSquare(mapX, mapY));
	if (L0555_T_LordChaosThing.toUint16()) {
		L0551_ui_FluxcageCount = (L0554_aB_Fluxcages[0] = isFluxcageOnSquare(mapX - 1, mapY)) +
			(L0554_aB_Fluxcages[1] = isFluxcageOnSquare(mapX + 1, mapY)) +
			(L0554_aB_Fluxcages[2] = isFluxcageOnSquare(mapX, mapY - 1)) +
			(L0554_aB_Fluxcages[3] = isFluxcageOnSquare(mapX, mapY + 1));
		while (L0551_ui_FluxcageCount++ < 4) {
			L0548_i_MapX = mapX;
			L0549_i_MapY = mapY;
			L0552_ui_FluxcageIndex = _vm->getRandomNumber(4);
			for (L0553_ui_Counter = 5; --L0553_ui_Counter; L0552_ui_FluxcageIndex = returnNextVal(L0552_ui_FluxcageIndex)) {
				if (!L0554_aB_Fluxcages[L0552_ui_FluxcageIndex]) {
					L0554_aB_Fluxcages[L0552_ui_FluxcageIndex] = true;
					switch (L0552_ui_FluxcageIndex) {
					case 0:
						L0548_i_MapX--;
						break;
					case 1:
						L0548_i_MapX++;
						break;
					case 2:
						L0549_i_MapY--;
						break;
					case 3:
						L0549_i_MapY++;
					}
					break;
				}
			}
			if (isSquareACorridorTeleporterPitOrDoor(L0548_i_MapX, L0549_i_MapY)) {
				if (!_vm->_moveSens->getMoveResult(L0555_T_LordChaosThing, mapX, mapY, L0548_i_MapX, L0549_i_MapY)) {
					startWanedring(L0548_i_MapX, L0549_i_MapY);
				}
				return;
			}
		}
		_vm->fuseSequence();
	}
}

void GroupMan::saveActiveGroupPart(Common::OutSaveFile* file) {
	for (uint16 i = 0; i < _maxActiveGroupCount; ++i) {
		ActiveGroup *group = &_activeGroups[i];
		file->writeUint16BE(group->_groupThingIndex);
		file->writeUint16BE(group->_directions);
		file->writeByte(group->_cells);
		file->writeByte(group->_lastMoveTime);
		file->writeByte(group->_delayFleeingFromTarget);
		file->writeByte(group->_targetMapX);
		file->writeByte(group->_targetMapY);
		file->writeByte(group->_priorMapX);
		file->writeByte(group->_priorMapY);
		file->writeByte(group->_homeMapX);
		file->writeByte(group->_homeMapY);
		for (uint16 j = 0; j < 4; ++j)
			file->writeByte(group->_aspect[j]);
	}
}

void GroupMan::loadActiveGroupPart(Common::InSaveFile* file) {
	for (uint16 i = 0; i < _maxActiveGroupCount; ++i) {
		ActiveGroup *group = &_activeGroups[i];
		group->_groupThingIndex = file->readUint16BE();
		group->_directions = (Direction)file->readUint16BE();
		group->_cells = file->readByte();
		group->_lastMoveTime = file->readByte();
		group->_delayFleeingFromTarget = file->readByte();
		group->_targetMapX = file->readByte();
		group->_targetMapY = file->readByte();
		group->_priorMapX = file->readByte();
		group->_priorMapY = file->readByte();
		group->_homeMapX = file->readByte();
		group->_homeMapY = file->readByte();
		for (uint16 j = 0; j < 4; ++j)
			group->_aspect[j] = file->readByte();
	}
}
}
