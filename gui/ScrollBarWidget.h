/* ScummVM - Scumm Interpreter
 * Copyright (C) 2002 The ScummVM project
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * $Header$
 */

#ifndef SCROLLBARWIDGET_H
#define SCROLLBARWIDGET_H

#include "widget.h"

enum {
	kScrollBarWidth		= 9
};

enum {
	kSetPositionCmd		= 'SETP'
};


class ScrollBarWidget : public Widget, public CommandSender {
protected:
	enum {
		kNoPart,
		kUpArrowPart,
		kDownArrowPart,
		kSliderPart,
		kPageUpPart,
		kPageDownPart
	}		_part;
	int		_sliderHeight;
	int		_sliderPos;

	bool	_isDraggingSlider;
	int		_sliderDeltaMouseDownPos;

public:
	int		_numEntries;
	int		_entriesPerPage;
	int		_currentPos;
public:
	ScrollBarWidget(Dialog *boss, int x, int y, int w, int h);
//	virtual ~ScrollBarWidget();

	void handleClick(int x, int y, int button);
	void handleMouseMoved(int x, int y, int button);
	void handleMouseEntered(int button)	{ setFlags(WIDGET_HILITED); }
	void handleMouseLeft(int button)	{ clearFlags(WIDGET_HILITED); _part = kNoPart; _isDraggingSlider = false; draw(); }

	void recalc();

protected:
	void drawWidget(bool hilite);
	void checkbounds();
};


#endif
