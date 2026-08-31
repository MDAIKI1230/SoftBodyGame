#pragma once

#include "PhysicsWorld.h"

class PhysicsAPI
{
public:
	// --- Cast系 ---

	static bool RayCastHit(const Ray& _ray, RayCastHitInfo& _hitInfo);


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
