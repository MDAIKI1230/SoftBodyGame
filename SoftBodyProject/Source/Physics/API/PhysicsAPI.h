#pragma once

#include "PhysicsWorld.h"

#include "Ray.h"
#include "RayCastHitInfo.h"

class PhysicsAPI
{
public:
	// --- Cast系 ---

	static bool RayCastHit(const Ray& _ray, RayCastHitInfo& _hitInfo);
	static bool RayCastHit(const Ray& _ray, RayCastHitInfo& _hitInfo, const CollisionFilter& _filter);


	static void BindWorld(PhysicsWorld& _physicsWorld);
	static void UnbindWorld();

private:
	static PhysicsWorld* physicsWorld;
	static ColliderStorage* colliderStorage;
	static BodyStorage* bodyStorage;
	static PhysicsTransformStorage* transformStorage;
	static ConstraintStorage* constraintStorage;

	PhysicsAPI();
};
