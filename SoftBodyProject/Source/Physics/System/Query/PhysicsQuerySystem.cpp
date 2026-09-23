#include "RayCastFunction.h"

#include "PhysicsQuerySystem.h"

bool PhysicsQuerySystem::RayCastHit(const Ray& _ray, RayCastHitInfo& _hitInfo, ColliderStorage* _colliderStorage, PhysicsTransformStorage* _transformStorage, const CollisionFilter& _filter)
{
	RayCastQueryHitInfo info;
	if (RayCastHit(_ray, info, _colliderStorage, _transformStorage, _filter))
	{
		_hitInfo.point = info.point;
		_hitInfo.normal = info.normal;
		_hitInfo.distance = info.distance;

		return true;
	}

	return false;
}

bool PhysicsQuerySystem::RayCastHit(const Ray& _ray, RayCastQueryHitInfo& _hitInfo, ColliderStorage* _colliderStorage, PhysicsTransformStorage* _transformStorage, const CollisionFilter& _filter)
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

		ColliderID colliderID{ aabb.colliderID };
		if (!CanCollide(_filter, _colliderStorage->GetFilter(colliderID)))
		{
			continue;
		}

		PhysicsTransformID transformID{ aabb.transformID };

		Vector3 position{ _transformStorage->GetPosition(transformID) };

		Vector3 worldMin{ aabb.min + position };
		Vector3 worldMax{ aabb.max + position };

		float aabbEnter{};

		if (!RayCastFunction::AABB(ray, worldMin, worldMax, bestDistance, aabbEnter))
		{
			continue;
		}

		// 実際の形状で当たり判定をする

		RayCastHitInfo hitInfo{};

		if (!RayCastCollider(ray, hitInfo, aabb.colliderID, transformID, _colliderStorage, _transformStorage))
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
	const Ray& _ray, RayCastHitInfo& _hitInfo, ColliderID _colliderID, PhysicsTransformID transformID,
	ColliderStorage* _colliderStorage, PhysicsTransformStorage* _transformStorage)
{
	ColliderType type{ _colliderStorage->GetType(_colliderID) };

	const Vector3& position{ _transformStorage->GetPosition(transformID) };
	const Quaternion& rotation{ _transformStorage->GetRotation(transformID) };
	const Vector3& scale{ _transformStorage->GetScale(transformID) };
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

bool PhysicsQuerySystem::CanCollide(const CollisionFilter& _aFilter, const CollisionFilter& _bFilter)
{
	// 種類単位の判定
	if ((_aFilter.collideMask & _bFilter.categoryBits) == 0 ||
		(_bFilter.collideMask & _aFilter.categoryBits) == 0)
	{
		return false;
	}

	// 同じ構造物内部の判定
	if (_aFilter.groupID != CollisionFilter::INVALID_COLLISION_GROUP &&
		_aFilter.groupID == _bFilter.groupID)
	{
		assert(_aFilter.memberIndex < 64);
		assert(_bFilter.memberIndex < 64);

		// bit情報に変換
		uint64_t bitA{ 1ull << _aFilter.memberIndex };
		uint64_t bitB{ 1ull << _bFilter.memberIndex };

		// お互い無視相手じゃないかチェック
		if ((_aFilter.ignoreMembers & bitB) != 0 ||
			(_bFilter.ignoreMembers & bitA) != 0)
		{
			return false;
		}
	}

	return true;
}