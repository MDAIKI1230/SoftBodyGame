#pragma once

#include <vector>

#include "ColliderID.h"
#include "PhysicsTransformID.h"
#include "BodyID.h"

class SphereColliderStorage
{
public:
	ColliderID Remove(size_t _index)
	{
		// 最後と入れ替えて最後を捨てる
		radius[_index] = radius.back();
		radius.pop_back();
		id[_index] = id.back();
		id.pop_back();
		transformID[_index] = transformID.back();
		transformID.pop_back();

		return id[_index];
	}
public:
	std::vector<float> radius;
	std::vector<ColliderID> id;
	std::vector<PhysicsTransformID> transformID;
};
