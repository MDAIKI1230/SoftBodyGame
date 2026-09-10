#include "TransformComponentStorage.h"

#include "SynchronizationSystem.h"

void SynchronizationSystem::Sync(WorldStorage* _worldStorage,  PhysicsTransformStorage* _physicsTransformStorage)
{
	// ストレージの取得
	TransformComponentStorage* transformStorage{ static_cast<TransformComponentStorage*>(_worldStorage->GetStorage<TransformComponent>()) };

	// PhysicsTransformの変更
	for (auto& id : _physicsTransformStorage->GetIDRange())
	{
		if (HasFlag(_physicsTransformStorage->GetSyncPolicy(id), PhysicsTransformSyncPolicy::READ_FROM_ECS))
		{
			TransformComponent& trans{ transformStorage->Edit(_physicsTransformStorage->GetOwnerEntity(id)) };

			_physicsTransformStorage->EditPosition(id) = trans.GetPosition();
			_physicsTransformStorage->EditRotation(id) = trans.GetRotation();
			_physicsTransformStorage->EditScale(id) = trans.GetScale();
			_physicsTransformStorage->EditLocalMatrix(id) = trans.GetLocalMatrix();
			_physicsTransformStorage->EditWorldMatrix(id) = trans.GetWorldMatrix();
		}
	}
}
