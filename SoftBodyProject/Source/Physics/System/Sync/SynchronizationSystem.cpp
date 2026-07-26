#include "TransformComponentStorage.h"

#include "SynchronizationSystem.h"

void SynchronizationSystem::Sync(WorldStorage* _worldStorage,  PhysicsTransformStorage* _physicsTransformStorage)
{
	// ストレージの取得
	TransformComponentStorage* transformStorage{ static_cast<TransformComponentStorage*>(_worldStorage->GetStorage<TransformComponent>()) };

	// PhysicsTransformの変更
	for (auto& id : _physicsTransformStorage->GetIDRange())
	{
		TransformComponent& trans{ transformStorage->Edit(_physicsTransformStorage->GetOwnerEntity(id)) };
		uint32_t index{ _physicsTransformStorage->GetDenseIndex(id) };
		_physicsTransformStorage->EditPosition(index) = trans.GetPosition();
		_physicsTransformStorage->EditRotation(index) = trans.GetRotation();
		_physicsTransformStorage->EditScale(index) = trans.GetScale();
		_physicsTransformStorage->EditLocalMatrix(index) = trans.GetLocalMatrix();
		_physicsTransformStorage->EditWorldMatrix(index) = trans.GetWorldMatrix();
	}
}
