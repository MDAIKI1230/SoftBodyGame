#include "CollisionConstants.h"

#include "TransformComponentStorage.h"

#include "AABBUpdateSystem.h"

void AABBUpdateSystem::FixedUpdate(PhysicsTransformStorage* _transformStorage, ColliderStorage* _colliderStorage)
{
	uint32_t size{ _colliderStorage->AABBCount() };
	for (uint32_t i{ 0 }; i < size; i++)
	{
		if (_colliderStorage->GetAABBDiaryFlag(i) & AABBChangeDiaryFlag::MAKE)
		{
			ColliderID id{ _colliderStorage->GetAABBBroadPhaseCollider(i).colliderID};

			switch (_colliderStorage->GetType(id))
			{
			case ColliderType::SPHERE:
				ComputeSphere(_colliderStorage->EditAABBBroadPhaseCollider(i), _colliderStorage, id, _transformStorage);
				break;
			case ColliderType::BOX:
				ComputeBox(_colliderStorage->EditAABBBroadPhaseCollider(i), _colliderStorage, id, _transformStorage);
				break;
			case ColliderType::CAPSULE:
				ComputeCapsule(_colliderStorage->EditAABBBroadPhaseCollider(i), _colliderStorage, id, _transformStorage);
				break;
			default:
				break;
			}

			_colliderStorage->EditAABBDiaryFlag(i) = AABBChangeDiaryFlag::NONE;
		}

		if (_colliderStorage->GetAABBDiaryFlag(i) & AABBChangeDiaryFlag::TRANSFORM)
		{
			ColliderID id{ _colliderStorage->GetAABBBroadPhaseCollider(i).colliderID };

			switch (_colliderStorage->GetType(id))
			{
			case ColliderType::SPHERE:
				ComputeSphere(_colliderStorage->EditAABBBroadPhaseCollider(i), _colliderStorage, id, _transformStorage);
				break;
			case ColliderType::BOX:
				ComputeBox(_colliderStorage->EditAABBBroadPhaseCollider(i), _colliderStorage, id, _transformStorage);
				break;
			case ColliderType::CAPSULE:
				ComputeCapsule(_colliderStorage->EditAABBBroadPhaseCollider(i), _colliderStorage, id, _transformStorage);
				break;
			default:
				break;
			}

			_colliderStorage->EditAABBDiaryFlag(i) = AABBChangeDiaryFlag::NONE;
		}

		if (_colliderStorage->GetAABBDiaryFlag(i) & AABBChangeDiaryFlag::SHAPE)
		{

		}
	}
}

void AABBUpdateSystem::ComputeSphere(AABBBroadPhaseCollider& _aabb, ColliderStorage* _colliderStorage, ColliderID _id, PhysicsTransformStorage* _transformStorage)
{
	Vector3 scale{ _transformStorage->GetScale(_transformStorage->GetDenseIndex(_colliderStorage->GetTransformID(_id))) };
	// 最大値で倍にする
	float multiple{ std::max(std::max(scale.x,scale.y),scale.z) };
	_aabb.min = Vector3{ -_colliderStorage->GetSphereColliderRadius(_id) * multiple };
	_aabb.max = Vector3{ _colliderStorage->GetSphereColliderRadius(_id) * multiple };
}

void AABBUpdateSystem::ComputeBox(AABBBroadPhaseCollider& _aabb, ColliderStorage* _colliderStorage, ColliderID _id, PhysicsTransformStorage* _transformStorage)
{
	uint32_t transIndex{ _transformStorage->GetDenseIndex(_colliderStorage->GetTransformID(_id)) };

	// 行列から各方向を取得
	Vector3 right = _transformStorage->GetRotation(transIndex).Rotate(Vector3::RIGHT);
	Vector3 up = _transformStorage->GetRotation(transIndex).Rotate(Vector3::UP);
	Vector3 forward = _transformStorage->GetRotation(transIndex).Rotate(Vector3::FORWARD);

	Vector3 halfScale{ _colliderStorage->GetBoxColliderScale(_id) * 0.5f};

	// 各方向に倍
	halfScale = SIMDVectorMath::Mul(halfScale, _transformStorage->GetScale(transIndex));

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

	_aabb.min = -aabbScale;
	_aabb.max = aabbScale;
}

void AABBUpdateSystem::ComputeCapsule(AABBBroadPhaseCollider& _aabb, ColliderStorage* _colliderStorage, ColliderID _id, PhysicsTransformStorage* _transformStorage)
{
	float radius{ _colliderStorage->GetCapsuleColliderRadius(_id) };
	float height{ _colliderStorage->GetCapsuleColliderHeight(_id) };
	_aabb.min = Vector3{ -radius,-height / 2.0f,-radius };
	_aabb.max = Vector3{ radius,height / 2.0f,radius };
}
