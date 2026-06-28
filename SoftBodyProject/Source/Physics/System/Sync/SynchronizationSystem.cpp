#include "TransformComponentStorage.h"

#include "SynchronizationSystem.h"

void SynchronizationSystem::Sync(WorldStorage* _worldStorage, RigidBodyStorage* _bodyStorage)
{
	// ストレージの取得
	TransformComponentStorage* transformStorage{ static_cast<TransformComponentStorage*>(_worldStorage->GetStorage<TransformComponent>()) };

	for (int i{ 0 }; i < _bodyStorage->id.size(); i++)
	{
		TransformComponent* trans{ transformStorage->Get(_bodyStorage->GetOwnerEntity(_bodyStorage->id[i])) };
		_bodyStorage->position[i] = trans->GetPosition();
		_bodyStorage->rotation[i] = trans->GetRotation();
		_bodyStorage->scale[i] = trans->GetScale();
	}
}
