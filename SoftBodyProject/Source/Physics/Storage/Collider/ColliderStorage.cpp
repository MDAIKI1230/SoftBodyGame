#include "ColliderStorage.h"

ColliderStorage::ColliderStorage()
{
	aabbStorage = std::make_unique<AABBBroadPhaseColliderStorage>();
	sphereStorage = std::make_unique<SphereColliderStorage>();
	boxStorage = std::make_unique<BoxColliderStorage>();
}

ColliderID ColliderStorage::CreateSphere(EntityID _entity, PhysicsTransformID _transformID, float _radius)
{
	// ColliderIDの作成(denseIndexに関しては、どの配列も同じサイズのためIDを使う)
	ColliderID id{ GenerateColliderID(ColliderType::SPHERE, sphereStorage->id.size(), _entity) };

	// 実際のデータを追加
	sphereStorage->radius.push_back(_radius);
	sphereStorage->id.push_back(id);
	sphereStorage->transformID.push_back(_transformID);

	// aabbを作成フラグを追加しておく(後からシステムが作ってくれる)
	aabbStorage->dirty.emplace_back(AABBChangeDirtyFlag::MAKE);
	aabbStorage->aabb.emplace_back(id, _transformID);

	// IDを返してあげる
	return id;
}

ColliderID ColliderStorage::CreateBox(EntityID _entity, PhysicsTransformID _transformID, const Vector3& _scale)
{
	// ColliderIDの作成(denseIndexに関しては、どの配列も同じサイズのためIDを使う)
	ColliderID id{ GenerateColliderID(ColliderType::BOX, boxStorage->id.size(), _entity) };

	// 実際のデータを追加
	boxStorage->scale.push_back(_scale);
	boxStorage->id.push_back(id);
	boxStorage->transformID.push_back(_transformID);

	// aabbを作成フラグを追加しておく(後からシステムが作ってくれる)
	aabbStorage->dirty.emplace_back(AABBChangeDirtyFlag::MAKE);
	aabbStorage->aabb.emplace_back(id, _transformID);

	return id;
}

void ColliderStorage::AttachBody(PhysicsTransformID _transformID, BodyID _bodyID)
{
	
}

void ColliderStorage::Destroy(ColliderID _id)
{
	// swap-removeしたときの移動したID
	ColliderID movedId;
	// データの削除
	switch (slots[_id.index].type)
	{
	case ColliderType::SPHERE:
		movedId = sphereStorage->Remove(slots[_id.index].denseIndex);
		// 移動後のIDの修正
		slots[movedId.index].denseIndex = slots[_id.index].denseIndex;
		break;
	case ColliderType::BOX:
		movedId = boxStorage->Remove(slots[_id.index].denseIndex);
		// 移動後のIDの修正
		slots[movedId.index].denseIndex = slots[_id.index].denseIndex;
		break;
	default:
		break;
	}
	// フリーに追加
	freeSlots.push_back(_id.index);
	slots[_id.index].generation++;
}

bool ColliderStorage::IsAlive(ColliderID _id) const
{
	return slots[_id.index].alive && slots[_id.index].generation == _id.generation;
}

ColliderType ColliderStorage::GetType(ColliderID _id) const
{
	return slots[_id.index].type;
}

size_t ColliderStorage::GetDenseIndex(ColliderID _id) const
{
	return slots[_id.index].denseIndex;
}

EntityID ColliderStorage::GetOwnerEntity(ColliderID _id) const
{
	return slots[_id.index].ownerEntity;
}

ColliderID ColliderStorage::GenerateColliderID(ColliderType _type, size_t _denseIndex, EntityID _ownerEntity)
{
	if (freeSlots.empty())
	{
		// --- フリーのスロットがないため新たにスロットを作成---
		
		// IDを作成(初代判定で1)
		ColliderID result{ slots.size(),1 };
		// Slotを増設
		slots.emplace_back(_type, _denseIndex, _ownerEntity);

		return result;
	}
	else
	{
		// フリーのスロットがあるためそれを使用

		// 最後を取る
		size_t index{ freeSlots.back() };
		freeSlots.pop_back();

		// 世代は削除時に加算済み
		slots[index].alive = true;
		slots[index].type = _type;
		slots[index].denseIndex = _denseIndex;
		slots[index].ownerEntity = _ownerEntity;

		// IDを作成(初代判定で1)
		return ColliderID{ (uint32_t)(index),slots[index].generation };
	}
}
