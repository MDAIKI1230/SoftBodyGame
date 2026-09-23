#include "Query/PhysicsQuerySystem.h"

#include "PhysicsAPI.h"

bool PhysicsAPI::RayCastHit(const Ray& _ray, RayCastHitInfo& _hitInfo)
{
	return PhysicsQuerySystem::RayCastHit(_ray, _hitInfo, colliderStorage, transformStorage);
}

bool PhysicsAPI::RayCastHit(const Ray& _ray, RayCastHitInfo& _hitInfo, const CollisionFilter& _filter)
{
	return PhysicsQuerySystem::RayCastHit(_ray, _hitInfo, colliderStorage, transformStorage, _filter);
}

void PhysicsAPI::BindWorld(PhysicsWorld& _physicsWorld)
{
	physicsWorld = &_physicsWorld;
	colliderStorage = _physicsWorld.GetColliderStorage();
	bodyStorage = _physicsWorld.GetRigidBodyStorage();
	transformStorage = _physicsWorld.GetPhysicsTransformStorage();
	constraintStorage = _physicsWorld.GetConstraintStorage();
}

void PhysicsAPI::UnbindWorld()
{
	physicsWorld = nullptr;
	colliderStorage = nullptr;
	bodyStorage = nullptr;
	transformStorage = nullptr;
	constraintStorage = nullptr;
}

PhysicsWorld* PhysicsAPI::physicsWorld{ nullptr };
ColliderStorage* PhysicsAPI::colliderStorage{ nullptr };
BodyStorage* PhysicsAPI::bodyStorage{ nullptr };
PhysicsTransformStorage* PhysicsAPI::transformStorage{ nullptr };
ConstraintStorage* PhysicsAPI::constraintStorage{ nullptr };
