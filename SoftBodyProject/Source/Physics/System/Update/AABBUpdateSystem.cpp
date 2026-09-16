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
	Vector3 scale{ _transformStorage->GetScale(_colliderStorage->GetTransformID(_id)) };
	// 最大値で倍にする
	float multiple{ std::max(std::max(std::abs(scale.x),std::abs(scale.y)),std::abs(scale.z)) };
	_aabb.min = Vector3{ -_colliderStorage->GetSphereColliderRadius(_id) * multiple };
	_aabb.max = Vector3{ _colliderStorage->GetSphereColliderRadius(_id) * multiple };
}

void AABBUpdateSystem::ComputeBox(AABBBroadPhaseCollider& _aabb, ColliderStorage* _colliderStorage, ColliderID _id, PhysicsTransformStorage* _transformStorage)
{
	PhysicsTransformID transID{ _colliderStorage->GetTransformID(_id) };

	// 行列から各方向を取得
	Vector3 right{ _transformStorage->GetRotation(transID).Rotate(Vector3::RIGHT) };
	Vector3 up{ _transformStorage->GetRotation(transID).Rotate(Vector3::UP) };
	Vector3 forward{ _transformStorage->GetRotation(transID).Rotate(Vector3::FORWARD) };

	right = _colliderStorage->GetBoxColliderOffsetRotation(_id).Rotate(right);
	up = _colliderStorage->GetBoxColliderOffsetRotation(_id).Rotate(up);
	forward = _colliderStorage->GetBoxColliderOffsetRotation(_id).Rotate(forward);

	Vector3 halfScale{ _colliderStorage->GetBoxColliderScale(_id) * 0.5f};

	const Vector3& scale{ _transformStorage->GetScale(transID) };
	Vector3 absScale{ std::abs(scale.x),std::abs(scale.y),std::abs(scale.z) };
	// 各方向に倍
	halfScale = SIMDVectorMath::Mul(halfScale, absScale);

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
	PhysicsTransformID transID{ _colliderStorage->GetTransformID(_id) };
	const Quaternion& rotation{ _transformStorage->GetRotation(transID) };

	Vector3 axisHalf{ rotation.Rotate(Vector3::UP * (height * 0.5f)) };

	axisHalf = _colliderStorage->GetBoxColliderOffsetRotation(_id).Rotate(axisHalf);

	Vector3 extent{
		std::abs(axisHalf.x) + radius,
		std::abs(axisHalf.y) + radius,
		std::abs(axisHalf.z) + radius
	};

	_aabb.min = -extent;
	_aabb.max = extent;
}
