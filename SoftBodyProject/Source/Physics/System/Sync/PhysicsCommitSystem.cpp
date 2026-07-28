#include "TransformComponentStorage.h"

#include "PhysicsCommitSystem.h"


void PhysicsCommitSystem::FixedUpdate(PhysicsTransformStorage* _physicsTransformStorage, WorldStorage* _worldStorage)
{
	// Transformストレージ
	TransformComponentStorage* transformStorage{ static_cast<TransformComponentStorage*>(_worldStorage->GetStorage<TransformComponent>()) };

	for (auto& id : _physicsTransformStorage->GetIDRange())
	{
		EntityID entity{ _physicsTransformStorage->GetOwnerEntity(id) };

		uint32_t denseIndex{ _physicsTransformStorage->GetDenseIndex(id) };

		transformStorage->Edit(entity).SetPosition(_physicsTransformStorage->GetPosition(denseIndex));
		transformStorage->Edit(entity).SetRotation(_physicsTransformStorage->GetRotation(denseIndex));
	}
}
