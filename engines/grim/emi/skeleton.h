/* ResidualVM - A 3D game interpreter
 *
 * ResidualVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 *
 */

#ifndef GRIM_SKELETON_H
#define GRIM_SKELETON_H

#include "engines/grim/object.h"

namespace Common {
class SeekableReadStream;
}

namespace Grim {
	
class AnimationEmi;
class Joint;

class Skeleton : public Object {
	int _numJoints;
	Joint *_joints;
	AnimationEmi *_anim;
	void loadSkeleton(Common::SeekableReadStream *data);
	void initBone(int index);
	void initBones();
public:
	Skeleton(const Common::String &filename, Common::SeekableReadStream *data);
	void resetAnim();
	void setAnim(AnimationEmi *anim);	
	int findJointIndex(Common::String name, int max);
};
	
} // end of namespace Grim

#endif
