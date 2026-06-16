#pragma once

#include "AABBBroadPhaseCollider.h"

class ColliderComponent
{
public:
	const Vector3& GetBroadMin() { return aabb.min; }
	const Vector3& GetBroadMax() { return aabb.max; }
protected:
	AABBBroadPhaseCollider aabb;
};