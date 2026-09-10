#include "TransformComponentStorage.h"

#include "PhysicsCommitSystem.h"


void PhysicsCommitSystem::FixedUpdate(PhysicsTransformStorage* _physicsTransformStorage, WorldStorage* _worldStorage)
{
	// Transformストレージ
	TransformComponentStorage* transformStorage{ static_cast<TransformComponentStorage*>(_worldStorage->GetStorage<TransformComponent>()) };

	for (auto& id : _physicsTransformStorage->GetIDRange())
	{
		if (HasFlag(_physicsTransformStorage->GetSyncPolicy(id), PhysicsTransformSyncPolicy::WRITE_TO_ECS))
		{
			EntityID entity{ _physicsTransformStorage->GetOwnerEntity(id) };

			transformStorage->Edit(entity).SetPosition(_physicsTransformStorage->GetPosition(id));
			transformStorage->Edit(entity).SetRotation(_physicsTransformStorage->GetRotation(id));
		}
	}
}
