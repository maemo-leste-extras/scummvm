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
 */

#ifndef MADS_NEBULAR_SCENES1_H
#define MADS_NEBULAR_SCENES1_H

#include "common/scummsys.h"
#include "mads/game.h"
#include "mads/scene.h"
#include "mads/nebular/nebular_scenes.h"

namespace MADS {

namespace Nebular {

class Scene1xx : public NebularScene {
protected:
	/**
	 * Plays an appropriate sound when entering a scene
	 */
	void sceneEntrySound();

	/**
	 *Sets the AA file to use for the scene
	 */
	void setAAName();

	/**
	 * Updates the prefix used for getting player sprites for the scene
	 */
	void setPlayerSpritesPrefix();
public:
	Scene1xx(MADSEngine *vm) : NebularScene(vm) {}
};

class Scene101: public Scene1xx {
private:
	bool _sittingFl;
	bool _panelOpened;

	int _messageNum;
	int _posY;
	int _shieldSpriteIdx;
	int _chairHotspotId;
	int _oldSpecial;

	void sayDang();
public:
	Scene101(MADSEngine *vm) : Scene1xx(vm), _sittingFl(false), _panelOpened(false), _messageNum(0), 
		_posY(0), _shieldSpriteIdx(0), _chairHotspotId(0), _oldSpecial(0) {}

	virtual void setup();
	virtual void enter();
	virtual void step();
	virtual void preActions();
	virtual void actions();
	virtual void postActions() {};
};

class Scene102 : public Scene1xx {
private:
	bool _fridgeOpenedFl;
	bool _fridgeOpenedDescr;
	bool _fridgeFirstOpenFl;
	bool _chairDescrFl;
	bool _drawerDescrFl;
	bool _activeMsgFl;

	int _fridgeCommentCount;

	void addRandomMessage();

public:
	Scene102(MADSEngine *vm) : Scene1xx(vm), _fridgeOpenedFl(false), _fridgeOpenedDescr(false), _fridgeFirstOpenFl(false),
		_chairDescrFl(false), _drawerDescrFl(false), _activeMsgFl(false), _fridgeCommentCount(0) {}

	virtual void setup();
	virtual void enter();
	virtual void step();
	virtual void preActions();
	virtual void actions();
	virtual void postActions();
};

class Scene103 : public Scene1xx {
public:
	Scene103(MADSEngine *vm) : Scene1xx(vm) {}

	virtual void setup();
	virtual void enter();
	virtual void step();
	virtual void preActions() {};
	virtual void actions();
	virtual void postActions();
};

class Scene104 : public Scene1xx {
private:
	bool _kargShootingFl;
	bool _loseFl;

public:
	Scene104(MADSEngine *vm) : Scene1xx(vm), _kargShootingFl(false), _loseFl(false) {}

	virtual void setup();
	virtual void enter();
	virtual void step();
	virtual void preActions();
	virtual void actions();
	virtual void postActions() {};
};

class Scene105 : public Scene1xx {
private:
	bool _explosionFl;

public:
	Scene105(MADSEngine *vm) : Scene1xx(vm), _explosionFl(false) {}

	virtual void setup();
	virtual void enter();
	virtual void step();
	virtual void preActions();
	virtual void actions();
	virtual void postActions() {};
};

class Scene106 : public Scene1xx {
private:
	bool _backToShipFl;
	bool _shadowFl;
	bool _firstEmergingFl;

	int _msgPosY;

public:
	Scene106(MADSEngine *vm) : Scene1xx(vm), _backToShipFl(false), _shadowFl(false), _firstEmergingFl(false), _msgPosY(0) {}

	virtual void setup();
	virtual void enter();
	virtual void step();
	virtual void preActions();
	virtual void actions();
	virtual void postActions() {};
};

class Scene107 : public Scene1xx {
private:
	bool _shootingFl;

public:
	Scene107(MADSEngine *vm) : Scene1xx(vm), _shootingFl(false) {}

	virtual void setup();
	virtual void enter();
	virtual void step();
	virtual void preActions();
	virtual void actions();
	virtual void postActions() {};
};

class Scene108 : public Scene1xx {
public:
	Scene108(MADSEngine *vm) : Scene1xx(vm) {}

	virtual void setup();
	virtual void enter();
	virtual void step() {};
	virtual void preActions();
	virtual void actions();
	virtual void postActions() {};
};

class Scene109 : public Scene1xx {
private:
	bool _rexThrowingObject;
	bool _hoovicDifficultFl;
	bool _beforeEatingRex;
	bool _eatingRex;
	bool _hungryFl;
	bool _eatingFirstFish;

	int _throwingObjectId;
	int _hoovicTrigger;

public:
	Scene109(MADSEngine *vm);

	virtual void setup();
	virtual void enter();
	virtual void step();
	virtual void preActions();
	virtual void actions();
	virtual void postActions() {};
};

class Scene110 : public Scene1xx {
private:
	int crabs_here;       /* Flag if crabs are here */

public:
	Scene110(MADSEngine *vm) : Scene1xx(vm) {}

	virtual void setup();
	virtual void enter();
	virtual void step();
	virtual void preActions();
	virtual void actions();
	virtual void postActions() {};
};

} // End of namespace Nebular
} // End of namespace MADS

#endif /* MADS_NEBULAR_SCENES1_H */
