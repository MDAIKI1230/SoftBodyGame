#pragma once

#include "Ray.h"
#include "RayCastHitInfo.h"
#include "RayCastQueryHitInfo.h"

#include "ColliderStorage.h"
#include "PhysicsTransformStorage.h"

class PhysicsQuerySystem
{
public:
	static bool RayCastHit(const Ray& _ray, RayCastHitInfo& _hitInfo, ColliderStorage* _colliderStorage, PhysicsTransformStorage* _transformStorage, const CollisionFilter& _filter = {});
	static bool RayCastHit(const Ray& _ray, RayCastQueryHitInfo& _hitInfo, ColliderStorage* _colliderStorage, PhysicsTransformStorage* _transformStorage, const CollisionFilter& _filter = {});
private:
	static bool RayCastCollider(
		const Ray& _ray, RayCastHitInfo& _hitInfo, ColliderID _colliderID, PhysicsTransformID transformID,
		ColliderStorage* _colliderStorage, PhysicsTransformStorage* _transformStorage);

	static bool CanCollide(const CollisionFilter& _aFilter, const CollisionFilter& _bFilter);
};
