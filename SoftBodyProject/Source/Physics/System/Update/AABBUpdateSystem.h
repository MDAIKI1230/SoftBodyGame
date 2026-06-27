#pragma once

#include "ColliderStorage.h"
#include "WorldStorage.h"

class AABBUpdateSystem
{
public:
	void FixedUpdate(ColliderStorage* _colliderStorage, WorldStorage* _worldStorage);
private:
	void ComputeSphere(AABBBroadPhaseCollider& aabb, SphereColliderStorage* _storage, size_t _index);
	void ComputeBox(AABBBroadPhaseCollider& aabb, ColliderStorage* _colliderStorage, size_t _index, WorldStorage* _worldStorage);
};
