#include "TransformComponentStorage.h"
#include "RigidBodyComponentStorage.h"

#include "PhysicsCommitSystem.h"


void PhysicsCommitSystem::FixedUpdate(RigidBodyStorage* _bodyStorage, WorldStorage* _worldStorage)
{
	// Transformストレージ
	TransformComponentStorage* transformStorage{ static_cast<TransformComponentStorage*>(_worldStorage->GetStorage<TransformComponent>()) };

	for (auto& id : _bodyStorage->id)
	{
		EntityID entity{ _bodyStorage->GetOwnerEntity(id) };

		transformStorage->Get(entity)->SetPosition(_bodyStorage->position[id.index]);
		transformStorage->Get(entity)->SetRotation(_bodyStorage->rotation[id.index]);
	}
}
