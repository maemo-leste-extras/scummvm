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

#include "common/config-manager.h"
#include "common/system.h"

#include "gui/saveload.h"
#include "gui/saveload-dialog.h"

#include "engines/metaengine.h"

namespace GUI {

SaveLoadChooser::SaveLoadChooser(const U32String &title, const U32String &buttonLabel, bool saveMode)
	: _impl(nullptr), _title(title), _buttonLabel(buttonLabel), _saveMode(saveMode) {
}

SaveLoadChooser::~SaveLoadChooser() {
	delete _impl;
	_impl = nullptr;
}

void SaveLoadChooser::selectChooser(const MetaEngineConnect &engine) {
#ifndef DISABLE_SAVELOADCHOOSER_GRID
	const SaveLoadChooserType requestedType = getRequestedSaveLoadDialog(engine);
	if (!_impl || _impl->getType() != requestedType) {
		delete _impl;
		_impl = nullptr;

		switch (requestedType) {
		case kSaveLoadDialogGrid:
			_impl = new SaveLoadChooserGrid(_title, _saveMode);
			break;

		default:
			// fallthrough intended
		case kSaveLoadDialogList:
#endif // !DISABLE_SAVELOADCHOOSER_GRID
			_impl = new SaveLoadChooserSimple(_title, _buttonLabel, _saveMode);
#ifndef DISABLE_SAVELOADCHOOSER_GRID
			break;
		}
	}
#endif // !DISABLE_SAVELOADCHOOSER_GRID
}

Common::String SaveLoadChooser::createDefaultSaveDescription(const int slot) const {
#if defined(USE_SAVEGAME_TIMESTAMP)
	TimeDate curTime;
	g_system->getTimeAndDate(curTime);
	curTime.tm_year += 1900; // fixup year
	curTime.tm_mon++; // fixup month
	return Common::String::format("%04d-%02d-%02d / %02d:%02d:%02d", curTime.tm_year, curTime.tm_mon, curTime.tm_mday, curTime.tm_hour, curTime.tm_min, curTime.tm_sec);
#else
	return Common::String::format("Save %d", slot + 1);
#endif
}

int SaveLoadChooser::runModalWithCurrentTarget() {
	const Plugin *plugin = EngineMan.findPlugin(ConfMan.get("engineid"));
	const Plugin *enginePlugin = nullptr;
	if (!plugin) {
		error("SaveLoadChooser::runModalWithCurrentTarget(): Cannot find plugin");
	} else {
		enginePlugin = PluginMan.giveEngineFromMetaEngine(plugin);

		if (!enginePlugin) {
			error("SaveLoadChooser::runModalWithCurrentTarget(): Couldn't match a Engine from the MetaEngine. \
				You will not be able to see savefiles until you have the necessary plugins.");
		}
	}
	return runModalWithPluginAndTarget(enginePlugin, ConfMan.getActiveDomainName());
}

int SaveLoadChooser::runModalWithPluginAndTarget(const Plugin *plugin, const String &target) {
	assert(plugin->getType() == PLUGIN_TYPE_ENGINE);

	selectChooser(plugin->get<MetaEngineConnect>());
	if (!_impl)
		return -1;

#if defined(USE_CLOUD) && defined(USE_LIBCURL)
	_impl->runSaveSync(ConfMan.hasKey("savepath", target));
#endif

	// Set up the game domain as newly active domain, so
	// target specific savepath will be checked
	String oldDomain = ConfMan.getActiveDomainName();
	ConfMan.setActiveDomain(target);

	int ret;
	do {
		ret = _impl->run(target, &plugin->get<MetaEngineConnect>());
#ifndef DISABLE_SAVELOADCHOOSER_GRID
		if (ret == kSwitchSaveLoadDialog) {
			selectChooser(plugin->get<MetaEngineConnect>());
		}
#endif // !DISABLE_SAVELOADCHOOSER_GRID
	} while (ret < -1);

	// Revert to the old active domain
	ConfMan.setActiveDomain(oldDomain);

	return ret;
}

const Common::U32String &SaveLoadChooser::getResultString() const {
	assert(_impl);
	return _impl->getResultString();
}

} // End of namespace GUI
