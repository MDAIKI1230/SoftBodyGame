#pragma once

#include "ColliderStorage.h"
#include "PhysicsTransformStorage.h"

class AABBUpdateSystem
{
public:
	void FixedUpdate(PhysicsTransformStorage* _transformStorage, ColliderStorage* _colliderStorage);
private:
	void ComputeSphere(AABBBroadPhaseCollider& aabb, ColliderStorage* _colliderStorage, ColliderID _id, PhysicsTransformStorage* _transformStorage);
	void ComputeBox(AABBBroadPhaseCollider& aabb, ColliderStorage* _colliderStorage, ColliderID _id, PhysicsTransformStorage* _transformStorage);
};
