#include "CollisionConstants.h"

#include "TransformComponentStorage.h"

#include "AABBUpdateSystem.h"

void AABBUpdateSystem::FixedUpdate(ColliderStorage* _colliderStorage, WorldStorage* _worldStorage)
{
	AABBBroadPhaseColliderStorage* aabbStorage{ _colliderStorage->aabbStorage.get()};
	for (int i{ 0 }; i < aabbStorage->dirty.size(); i++)
	{
		if (aabbStorage->dirty[i] & AABBChangeDirtyFlag::MAKE)
		{
			ColliderID id{ _colliderStorage->aabbStorage->aabb[i].colliderID };
			size_t index{ _colliderStorage->GetDenseIndex(id) };

			switch (_colliderStorage->GetType(id))
			{
			case ColliderType::SPHERE:
				ComputeSphere(_colliderStorage->aabbStorage->aabb[i], _colliderStorage->sphereStorage.get(), _colliderStorage->slots[id.index].denseIndex);
				break;
			case ColliderType::BOX:
				ComputeBox(_colliderStorage->aabbStorage->aabb[i], _colliderStorage, _colliderStorage->slots[id.index].denseIndex, _worldStorage);
				break;
			default:
				break;
			}

			aabbStorage->dirty[i] = AABBChangeDirtyFlag::NONE;
		}

		if (aabbStorage->dirty[i] & AABBChangeDirtyFlag::TRANSFORM)
		{

		}

		if (aabbStorage->dirty[i] & AABBChangeDirtyFlag::SHAPE)
		{

		}
	}
}

void AABBUpdateSystem::ComputeSphere(AABBBroadPhaseCollider& aabb, SphereColliderStorage* _storage, size_t _index)
{
	aabb.min = Vector3{ -_storage->radius[_index] };
	aabb.max = Vector3{ _storage->radius[_index] };
}

void AABBUpdateSystem::ComputeBox(AABBBroadPhaseCollider& aabb, ColliderStorage* _colliderStorage, size_t _index, WorldStorage* _worldStorage)
{
	ColliderID id{ _colliderStorage->boxStorage->id[_index] };
	TransformComponent* trans{ _worldStorage->GetStorage<TransformComponent>()->Get(_colliderStorage->GetOwnerEntity(id)) };

	Vector3 right{ trans->Right() };
	Vector3 up{ trans->Up() };
	Vector3 forward{ trans->Forward() };

	Vector3 halfScale{ _colliderStorage->boxStorage->scale[_index] * 0.5f };

	halfScale = SIMDVectorMath::Mul(halfScale, trans->GetScale());

	Vector3 aabbScale;

	aabbScale.x =
		std::abs(right.x) * halfScale.x +
		std::abs(up.x) * halfScale.y +
		std::abs(forward.x) * halfScale.z;

	aabbScale.y =
		std::abs(right.y) * halfScale.x +
		std::abs(up.y) * halfScale.y +
		std::abs(forward.y) * halfScale.z;

	aabbScale.z =
		std::abs(right.z) * halfScale.x +
		std::abs(up.z) * halfScale.y +
		std::abs(forward.z) * halfScale.z;

	aabb.min = aabbScale;
	aabb.max = aabbScale;
}
