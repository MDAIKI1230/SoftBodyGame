#pragma once

#include <vector>
#include "AABBBroadPhaseCollider.h"

// EntityをColliderIDとする
class AABBBroadPhaseColliderStorage
{
public:
	std::vector<AABBBroadPhaseCollider> aabb;
	std::vector<AABBDirtyFlag> dirty;
};
