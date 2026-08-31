#pragma once

#include "Ray.h"
#include "RayCastHitInfo.h"

#include "ColliderStorage.h"
#include "PhysicsTransformStorage.h"

class PhysicsQuerySystem
{
public:
	static bool RayCastHit(const Ray& _ray, RayCastHitInfo& _hitInfo, ColliderStorage* _colliderStorage, PhysicsTransformStorage* _transformStorage);
private:
	static bool RayCastCollider(
		const Ray& _ray, RayCastHitInfo& _hitInfo, ColliderID _colliderID, uint32_t transformIndex,
		ColliderStorage* _colliderStorage, PhysicsTransformStorage* _transformStorage);
};
