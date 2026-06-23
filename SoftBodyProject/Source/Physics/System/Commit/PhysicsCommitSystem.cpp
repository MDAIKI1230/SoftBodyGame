#include "TransformComponentStorage.h"
#include "RigidBodyComponentStorage.h"

#include "PhysicsCommitSystem.h"

PhysicsCommitSystem::PhysicsCommitSystem()
{
	priority = INT_MIN;
}

void PhysicsCommitSystem::FixedUpdate(IWorld* _world)
{
	// RigidBodyストレージ
	RigidBodyComponentStorage* bodyStorage{ static_cast<RigidBodyComponentStorage*>(_world->GetStorage<RigidBodyComponent>()) };
	// Transformストレージ
	TransformComponentStorage* transformStorage{ static_cast<TransformComponentStorage*>(_world->GetStorage<TransformComponent>()) };

	for (int entity : *bodyStorage->GetEntities())
	{
		RigidBodyComponent* component{ bodyStorage->Get(entity) };
		int handle{ component->GetHnadle() };

		transformStorage->Get(handle)->SetPosition(bodyStorage->expectedPos[handle]);
		transformStorage->Get(handle)->SetRotation(bodyStorage->expectedRot[handle]);
	}
}
