/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or(at your option) any later version.
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

#ifndef AGS_PLUGINS_AGS_SPRITE_FONT_AGS_SPRITE_FONT_H
#define AGS_PLUGINS_AGS_SPRITE_FONT_AGS_SPRITE_FONT_H

#include "ags/plugins/plugin_base.h"
#include "ags/plugins/ags_sprite_font/sprite_font_renderer.h"
#include "ags/plugins/ags_sprite_font/variable_width_sprite_font.h"

namespace AGS3 {
class IAGSEngine;
}

namespace AGS3 {
namespace Plugins {
namespace AGSSpriteFont {

class AGSSpriteFont : public PluginBase {
protected:
	static IAGSEngine *_engine;
	static SpriteFontRenderer *_fontRenderer;
	static VariableWidthSpriteFontRenderer *_vWidthRenderer;

protected:
	static const char *AGS_GetPluginName();
	static void AGS_EngineStartup(IAGSEngine *lpEngine);
	static void AGS_EngineShutdown();

private:
	static void SetSpriteFont(ScriptMethodParams &params);
	static void SetVariableSpriteFont(ScriptMethodParams &params);
	static void SetGlyph(ScriptMethodParams &params);
	static void SetSpacing(ScriptMethodParams &params);
	static void SetLineHeightAdjust(ScriptMethodParams &params);

public:
	AGSSpriteFont();
};

} // namespace AGSSpriteFont
} // namespace Plugins
} // namespace AGS3

#endif
