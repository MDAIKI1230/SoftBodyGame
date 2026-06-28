#pragma once

#pragma once

#include <vector>

#include "ColliderID.h"
#include "PhysicsTransformID.h"
#include "BodyID.h"

class BoxColliderStorage
{
public:
	ColliderID Remove(size_t _index)
	{
		// 最後と入れ替えて最後を捨てる
		scale[_index] = scale.back();
		scale.pop_back();
		id[_index] = id.back();
		id.pop_back();
		transformID[_index] = transformID.back();
		transformID.pop_back();

		return id[_index];
	}
public:
	std::vector<Vector3> scale;
	std::vector<ColliderID> id;
	std::vector<PhysicsTransformID> transformID;
};
