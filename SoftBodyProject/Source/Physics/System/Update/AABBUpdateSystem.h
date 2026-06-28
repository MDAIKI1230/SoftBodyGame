#pragma once

#include "ColliderStorage.h"
#include "PhysicsTransformStorage.h"

class AABBUpdateSystem
{
public:
	void FixedUpdate(PhysicsTransformStorage* _transformStorage, ColliderStorage* _colliderStorage);
private:
	void ComputeSphere(AABBBroadPhaseCollider& aabb, SphereColliderStorage* _sphereStorage, size_t _index, PhysicsTransformStorage* _transformStorage);
	void ComputeBox(AABBBroadPhaseCollider& aabb, BoxColliderStorage* _boxStorage, size_t _index, PhysicsTransformStorage* _transformStorage);
};
