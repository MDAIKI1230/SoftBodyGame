#include "TransformComponentStorage.h"

#include "SynchronizationSystem.h"

void SynchronizationSystem::Sync(WorldStorage* _worldStorage,  PhysicsTransformStorage* _physicsTransformStorage)
{
	// ストレージの取得
	TransformComponentStorage* transformStorage{ static_cast<TransformComponentStorage*>(_worldStorage->GetStorage<TransformComponent>()) };

	// PhysicsTransformの変更
	for (int i{ 0 }; i < _physicsTransformStorage->id.size(); i++)
	{
		TransformComponent* trans{ transformStorage->Get(_physicsTransformStorage->GetOwnerEntity(_physicsTransformStorage->id[i])) };
		_physicsTransformStorage->EditPosition(i) = trans->GetPosition();
		_physicsTransformStorage->rotation[i] = trans->GetRotation();
		_physicsTransformStorage->scale[i] = trans->GetScale();
		_physicsTransformStorage->localMatrix[i] = trans->GetLocalMatrix();
		_physicsTransformStorage->worldMatrix[i] = trans->GetWorldMatrix();
	}
}
