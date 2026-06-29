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
	ColliderID id{ GenerateColliderID(ColliderType::SPHERE, sphereStorage->id.size(), _entity, _transformID) };

	// 実際のデータを追加
	sphereStorage->radius.push_back(_radius);
	sphereStorage->id.push_back(id);

	// aabbを作成フラグを追加しておく(後からシステムが作ってくれる)
	aabbStorage->dirty.emplace_back(AABBChangeDirtyFlag::MAKE);
	aabbStorage->aabb.emplace_back(id, _transformID);

	// mapに追加
	transformMap[_transformID].push_back(id);

	// IDを返してあげる
	return id;
}

ColliderID ColliderStorage::CreateBox(EntityID _entity, PhysicsTransformID _transformID, const Vector3& _scale)
{
	// ColliderIDの作成(denseIndexに関しては、どの配列も同じサイズのためIDを使う)
	ColliderID id{ GenerateColliderID(ColliderType::BOX, boxStorage->id.size(), _entity, _transformID) };

	// 実際のデータを追加
	boxStorage->scale.push_back(_scale);
	boxStorage->id.push_back(id);

	// aabbを作成フラグを追加しておく(後からシステムが作ってくれる)
	aabbStorage->dirty.emplace_back(AABBChangeDirtyFlag::MAKE);
	aabbStorage->aabb.emplace_back(id, _transformID);

	// mapに追加
	transformMap[_transformID].push_back(id);

	return id;
}

void ColliderStorage::Destroy(ColliderID _id)
{
	if (!IsAlive(_id))
	{
		return;
	}

	// Mapから削除
	auto it = transformMap.find(slots[_id.index].transformID);
	if (it != transformMap.end())
	{
		auto& list = it->second;

		std::erase_if(list, [_id](const ColliderID& x)
			{
				return x.index == _id.index && x.generation == _id.generation;
			});

		if (list.empty())
		{
			transformMap.erase(it);
		}
	}

	// 移動インデックス
	ColliderID movedId{};

	switch (slots[_id.index].type)
	{
	case ColliderType::SPHERE:
		movedId = sphereStorage->Remove(slots[_id.index].denseIndex);
		break;

	case ColliderType::BOX:
		movedId = boxStorage->Remove(slots[_id.index].denseIndex);
		break;
	}

	if (!(movedId.index == _id.index && movedId.generation == _id.generation))
	{
		slots[movedId.index].denseIndex = slots[_id.index].denseIndex;
	}

	// 削除
	slots[_id.index].alive = false;
	slots[_id.index].generation++;
	freeSlots.push_back(_id.index);
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

PhysicsTransformID ColliderStorage::GetTransformID(ColliderID _id) const
{
	return slots[_id.index].transformID;
}

ColliderID ColliderStorage::GenerateColliderID(ColliderType _type, size_t _denseIndex, EntityID _ownerEntity, PhysicsTransformID _transformID)
{
	if (freeSlots.empty())
	{
		// --- フリーのスロットがないため新たにスロットを作成---
		
		// IDを作成(初代判定で1)
		ColliderID result{ slots.size(),1 };
		// Slotを増設
		slots.emplace_back(_type, _denseIndex, _ownerEntity, _transformID);

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
