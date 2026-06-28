#include "CollisionConstants.h"

#include "TransformComponentStorage.h"

#include "AABBUpdateSystem.h"

void AABBUpdateSystem::FixedUpdate(PhysicsTransformStorage* _transformStorage, ColliderStorage* _colliderStorage)
{
	AABBBroadPhaseColliderStorage* aabbStorage{ _colliderStorage->aabbStorage.get() };
	for (int i{ 0 }; i < aabbStorage->dirty.size(); i++)
	{
		if (aabbStorage->dirty[i] & AABBChangeDirtyFlag::MAKE)
		{
			ColliderID id{ _colliderStorage->aabbStorage->aabb[i].colliderID };
			size_t index{ _colliderStorage->GetDenseIndex(id) };

			switch (_colliderStorage->GetType(id))
			{
			case ColliderType::SPHERE:
				ComputeSphere(_colliderStorage->aabbStorage->aabb[i], _colliderStorage->sphereStorage.get(), _colliderStorage->slots[id.index].denseIndex, _transformStorage);
				break;
			case ColliderType::BOX:
				ComputeBox(_colliderStorage->aabbStorage->aabb[i], _colliderStorage->boxStorage.get(), _colliderStorage->slots[id.index].denseIndex, _transformStorage);
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

void AABBUpdateSystem::ComputeSphere(AABBBroadPhaseCollider& aabb, SphereColliderStorage* _sphereStorage, size_t _index, PhysicsTransformStorage* _transformStorage)
{
	Vector3& scale{ _transformStorage->scale[_transformStorage->GetDenseIndex(_sphereStorage->transformID[_index])] };
	// 最大値で倍にする
	float multiple{ std::max(std::max(scale.x,scale.y),scale.z) };
	aabb.min = Vector3{ -_sphereStorage->radius[_index] * multiple };
	aabb.max = Vector3{ _sphereStorage->radius[_index] * multiple };
}

void AABBUpdateSystem::ComputeBox(AABBBroadPhaseCollider& aabb, BoxColliderStorage* _boxStorage, size_t _index, PhysicsTransformStorage* _transformStorage)
{
	ColliderID id{ _boxStorage->id[_index] };

	uint32_t transIndex{ _transformStorage->GetDenseIndex(_boxStorage->transformID[_index]) };

	// 行列から各方向を取得
	Vector3 right{ _transformStorage->worldMatrix[transIndex] * Vector3::RIGHT };
	Vector3 up{ _transformStorage->worldMatrix[transIndex] * Vector3::UP };
	Vector3 forward{ _transformStorage->worldMatrix[transIndex] * Vector3::FORWARD };

	Vector3 halfScale{ _boxStorage->scale[_index] * 0.5f };

	// 各方向に倍
	halfScale = SIMDVectorMath::Mul(halfScale, _transformStorage->scale[transIndex]);

	Vector3 aabbScale;

	// スケールを計算
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

	aabb.min = -aabbScale;
	aabb.max = aabbScale;
}
