#include "TransformComponent.h"
#include "ColliderComponentStorage.h"

#include "CollisionSystem.h"

void CollisionSystem::FixedUpdate(IWorld* world)
{
	// コライダーストレージ
	SparseSetStorageBase<ColliderComponent>* colliderStorage{ world->GetStorage<ColliderComponent>() };
	// Transformストレージ
	SparseSetStorageBase<TransformComponent>* transformStorage{ world->GetStorage<TransformComponent>() };
}

void CollisionSystem::BroadPhase(TransformComponentStorage* transformStorage, ColliderComponentStorage* colliderStorage)
{
	// 参照用
	TransformComponent trans{};

	for (int entity : *colliderStorage->GetEntities())
	{
		// Transformがあるかチェックないなら飛ばす
		if (!transformStorage->TryGet(entity, trans))
		{
			continue;
		}

		// 各軸に射影し、
	}
}

void CollisionSystem::NarrowPhase(TransformComponentStorage* transformStorage, ColliderComponentStorage* colliderStorage)
{

}