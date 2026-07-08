#pragma once

#include "MDMath.h"
#include "ColliderID.h"
#include "PhysicsTransformID.h"

struct AABBBroadPhaseCollider
{
public:
	// コンストラクタ
	AABBBroadPhaseCollider() = default;
	// コンストラクタ
	AABBBroadPhaseCollider(ColliderID _colliderID, PhysicsTransformID _transformID) :
		colliderID{ _colliderID },
		transformID{ _transformID }
	{
	}
public:
	// 最小
	Vector3 min;
	// 最大
	Vector3 max;
	// コライダーハンドル(どのコライダーのAABBかの識別用)
	ColliderID colliderID;
	// TransformID
	PhysicsTransformID transformID;
};
