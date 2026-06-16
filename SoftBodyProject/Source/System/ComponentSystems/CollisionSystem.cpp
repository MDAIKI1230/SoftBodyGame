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

void CollisionSystem::BroadPhase(TransformComponentStorage* transformStorage, std::vector<int>* colliderEntities)
{
	// 参照用
	TransformComponent trans{};

	for (int entity : *colliderEntities)
	{

	}
}

void CollisionSystem::NarrowPhase(TransformComponentStorage* transformStorage, std::vector<int>* colliderEntities)
{

}