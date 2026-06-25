#include "TransformComponentStorage.h"
#include "RigidBodyComponentStorage.h"

#include "PhysicsCommitSystem.h"

PhysicsCommitSystem::PhysicsCommitSystem()
{
	priority = INT_MIN;
}

void PhysicsCommitSystem::FixedUpdate(WorldStorage* _worldStorage, EventManager* _eventManager)
{
	// RigidBodyストレージ
	RigidBodyComponentStorage* bodyStorage{ static_cast<RigidBodyComponentStorage*>(_worldStorage->GetStorage<RigidBodyComponent>()) };
	// Transformストレージ
	TransformComponentStorage* transformStorage{ static_cast<TransformComponentStorage*>(_worldStorage->GetStorage<TransformComponent>()) };

	for (int entity : *bodyStorage->GetEntities())
	{
		RigidBodyComponent* component{ bodyStorage->Get(entity) };
		int handle{ component->GetHnadle() };

		transformStorage->Get(handle)->SetPosition(bodyStorage->expectedPos[handle]);
		transformStorage->Get(handle)->SetRotation(bodyStorage->expectedRot[handle]);
	}
}
