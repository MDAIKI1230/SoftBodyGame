#pragma once

#include "MDMath.h"
#include "ColliderID.h"

struct AABBBroadPhaseCollider
{
	// 最小
	Vector3 min;
	// 最大
	Vector3 max;
	// コライダーハンドル(どのコライダーのAABBかの識別用)
	ColliderID colliderID;
};
