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
	ColliderID id{ GenerateColliderID(ColliderType::SPHERE, sphereStorage->CountID(),aabbStorage->aabb.size(), _entity, _transformID)};

	// 実際のデータを追加
	sphereStorage->Add(id, _radius);

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
	ColliderID id{ GenerateColliderID(ColliderType::BOX, boxStorage->CountID(),aabbStorage->aabb.size(), _entity, _transformID) };

	// 実際のデータを追加
	boxStorage->Add(id, _scale);

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
	auto it = transformMap.find(slots[_id.GetIndex()].transformID);
	if (it != transformMap.end())
	{
		auto& list = it->second;

		std::erase_if(list, [_id](ColliderID x)
			{
				return x.GetIndex() == _id.GetIndex() && x.GetGeneration() == _id.GetGeneration();
			});

		if (list.empty())
		{
			transformMap.erase(it);
		}
	}

	// 移動インデックス
	ColliderID movedId{};

	switch (slots[_id.GetIndex()].type)
	{
	case ColliderType::SPHERE:
		movedId = sphereStorage->Remove(slots[_id.GetIndex()].denseIndex);
		break;

	case ColliderType::BOX:
		movedId = boxStorage->Remove(slots[_id.GetIndex()].denseIndex);
		break;
	}

	if (!(movedId.GetIndex() == _id.GetIndex() && movedId.GetGeneration() == _id.GetGeneration()))
	{
		slots[movedId.GetIndex()].denseIndex = slots[_id.GetIndex()].denseIndex;
	}

	// aabbも消す
	movedId = aabbStorage->Remove(slots[_id.GetIndex()].aabbIndex);
	if (!(movedId.GetIndex() == _id.GetIndex() && movedId.GetGeneration() == _id.GetGeneration()))
	{
		slots[movedId.GetIndex()].aabbIndex = slots[_id.GetIndex()].aabbIndex;
	}
	// 削除
	slots[_id.GetIndex()].alive = false;
	slots[_id.GetIndex()].generation++;
	freeSlots.push_back(_id.GetIndex());
}

bool ColliderStorage::IsAlive(ColliderID _id) const
{
	return slots[_id.GetIndex()].alive && slots[_id.GetIndex()].generation == _id.GetGeneration();
}

ColliderType ColliderStorage::GetType(ColliderID _id) const
{
	return slots[_id.GetIndex()].type;
}

uint32_t ColliderStorage::GetDenseIndex(ColliderID _id) const
{
	return slots[_id.GetIndex()].denseIndex;
}

uint32_t ColliderStorage::GetAABBIndex(ColliderID _id) const
{
	return slots[_id.GetIndex()].aabbIndex;
}

EntityID ColliderStorage::GetOwnerEntity(ColliderID _id) const
{
	return slots[_id.GetIndex()].ownerEntity;
}

PhysicsTransformID ColliderStorage::GetTransformID(ColliderID _id) const
{
	return slots[_id.GetIndex()].transformID;
}

std::vector<ColliderID>& ColliderStorage::GetColliderIDFromTransformID(PhysicsTransformID _id)
{
	return transformMap[_id];
}

ColliderID ColliderStorage::GenerateColliderID(ColliderType _type, uint32_t _denseIndex, uint32_t _aabbIndex,EntityID _ownerEntity, PhysicsTransformID _transformID)
{
	if (freeSlots.empty())
	{
		// --- フリーのスロットがないため新たにスロットを作成---
		
		// IDを作成(初代判定で1)
		ColliderID result{ static_cast<ColliderID::Index>(slots.size()),1 };
		// Slotを増設
		slots.emplace_back(_type, _denseIndex, _aabbIndex, _ownerEntity, _transformID);

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
		slots[index].aabbIndex = _aabbIndex;
		slots[index].ownerEntity = _ownerEntity;
		slots[index].transformID = _transformID;

		// IDを作成(初代判定で1)
		return ColliderID{ (uint32_t)(index),slots[index].generation };
	}
}
