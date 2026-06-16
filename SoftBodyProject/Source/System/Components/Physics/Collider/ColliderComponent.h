#pragma once

#include "AABBBroadPhaseCollider.h"

class ColliderComponent
{
public:
	float GetBroadWidth() { return aabb.width; }
	float GetBroadHeight() { return aabb.height; }
protected:
	AABBBroadPhaseCollider aabb;
};