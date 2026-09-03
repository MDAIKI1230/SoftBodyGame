#include "RayCastFunction.h"

#include "PhysicsQuerySystem.h"

bool PhysicsQuerySystem::RayCastHit(const Ray& _ray, RayCastHitInfo& _hitInfo, ColliderStorage* _colliderStorage, PhysicsTransformStorage* _transformStorage)
{
	RayCastQueryHitInfo info;
	if (RayCastHit(_ray, info, _colliderStorage, _transformStorage))
	{
		_hitInfo.point = info.point;
		_hitInfo.normal = info.normal;
		_hitInfo.distance = info.distance;

		return true;
	}

	return false;
}

bool PhysicsQuerySystem::RayCastHit(const Ray& _ray, RayCastQueryHitInfo& _hitInfo, ColliderStorage* _colliderStorage, PhysicsTransformStorage* _transformStorage)
{
	bool found{ false };
	float bestDistance{ _ray.maxDistance };
	RayCastQueryHitInfo bestHit{};

	Ray ray{ _ray };
	ray.direction.Normalize();

	for (uint32_t i = 0; i < _colliderStorage->AABBCount(); i++)
	{
		// AABBの判定
		const auto& aabb{ _colliderStorage->GetAABBBroadPhaseCollider(i) };

		uint32_t transformIndex{ _transformStorage->GetDenseIndex(aabb.transformID) };

		Vector3 position{ _transformStorage->GetPosition(transformIndex) };

		Vector3 worldMin{ aabb.min + position };
		Vector3 worldMax{ aabb.max + position };

		float aabbEnter{};

		if (!RayCastFunction::AABB(ray, worldMin, worldMax, bestDistance, aabbEnter))
		{
			continue;
		}

		// 実際の形状で当たり判定をする

		RayCastHitInfo hitInfo{};

		if (!RayCastCollider(ray, hitInfo, aabb.colliderID, transformIndex, _colliderStorage, _transformStorage))
		{
			continue;
		}

		// 当たっていたならベスト距離より近いなら値を更新する
		if (hitInfo.distance < bestDistance)
		{
			bestDistance = hitInfo.distance;
			bestHit = RayCastQueryHitInfo{ hitInfo.point,hitInfo.normal,hitInfo.distance,aabb.colliderID };
			found = true;
		}
	}

	// 当たったフラグが立っているなら情報を渡す
	if (found)
	{
		_hitInfo = bestHit;
	}

	return found;
}

bool PhysicsQuerySystem::RayCastCollider(
	const Ray& _ray, RayCastHitInfo& _hitInfo, ColliderID _colliderID, uint32_t transformIndex,
	ColliderStorage* _colliderStorage, PhysicsTransformStorage* _transformStorage)
{
	ColliderType type{ _colliderStorage->GetType(_colliderID) };

	const Vector3& position{ _transformStorage->GetPosition(transformIndex) };
	const Quaternion& rotation{ _transformStorage->GetRotation(transformIndex) };
	const Vector3& scale{ _transformStorage->GetScale(transformIndex) };
	switch (type)
	{
	case ColliderType::SPHERE:
		return RayCastFunction::Sphere(_ray, _hitInfo, position, scale, _colliderStorage->GetSphereColliderRadius(_colliderID));
	case ColliderType::BOX:
		return RayCastFunction::Box(_ray, _hitInfo, position, rotation, scale, _colliderStorage->GetBoxColliderScale(_colliderID) / 2.0f);
	case ColliderType::CAPSULE:
		return RayCastFunction::Capsule(_ray, _hitInfo, position, rotation,
			_colliderStorage->GetCapsuleColliderRadius(_colliderID), _colliderStorage->GetCapsuleColliderHeight(_colliderID));
	default:
		break;
	}

	return false;
}
