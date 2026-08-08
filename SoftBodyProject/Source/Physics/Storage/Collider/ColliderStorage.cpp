#include "ColliderStorage.h"

// コンストラクタ
ColliderStorage::ColliderStorage()
{
	aabbStorage = std::make_unique<AABBBroadPhaseColliderStorage>();
	sphereStorage = std::make_unique<SphereColliderStorage>();
	boxStorage = std::make_unique<BoxColliderStorage>();
	capsuleStorage = std::make_unique<CapsuleColliderStorage>();
}

// 球作成
ColliderID ColliderStorage::CreateSphere(EntityID _entity, PhysicsTransformID _transformID, float _radius)
{
	// ColliderIDの作成(denseIndexに関しては、どの配列も同じサイズのためIDを使う)
	ColliderID id{ CreateID(ColliderType::SPHERE, sphereStorage->CountID(),aabbStorage->Count(), _entity, _transformID)};

	// 実際のデータを追加
	sphereStorage->Add(id, _radius);

	// aabbを作成フラグを追加しておく(後からシステムが作ってくれる)
	aabbStorage->Add(AABBBroadPhaseCollider{ id, _transformID }, AABBChangeDiaryFlag::MAKE);

	// mapに追加
	transformMap[_transformID].push_back(id);

	// IDを返してあげる
	return id;
}

// ボックス作成
ColliderID ColliderStorage::CreateBox(EntityID _entity, PhysicsTransformID _transformID, const Vector3& _scale)
{
	// ColliderIDの作成(denseIndexに関しては、どの配列も同じサイズのためIDを使う)
	ColliderID id{ CreateID(ColliderType::BOX, boxStorage->CountID(),aabbStorage->Count(), _entity, _transformID)};

	// 実際のデータを追加
	boxStorage->Add(id, _scale);

	// aabbを作成フラグを追加しておく(後からシステムが作ってくれる)
	aabbStorage->Add(AABBBroadPhaseCollider{ id, _transformID }, AABBChangeDiaryFlag::MAKE);

	// mapに追加
	transformMap[_transformID].push_back(id);

	return id;
}

// カプセル作成
ColliderID ColliderStorage::CreateCapsule(EntityID _entity, PhysicsTransformID _transformID, float _height, float _radius)
{
	// ColliderIDの作成(denseIndexに関しては、どの配列も同じサイズのためIDを使う)
	ColliderID id{ CreateID(ColliderType::CAPSULE, capsuleStorage->CountID(),aabbStorage->Count(), _entity, _transformID) };

	// 実際のデータを追加
	capsuleStorage->Add(id, _height, _radius);

	// aabbを作成フラグを追加しておく(後からシステムが作ってくれる)
	aabbStorage->Add(AABBBroadPhaseCollider{ id, _transformID }, AABBChangeDiaryFlag::MAKE);

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
	auto it = transformMap.find(GetTransformID(_id));
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

	switch (GetType(_id))
	{
	case ColliderType::SPHERE:
		movedId = sphereStorage->Remove(GetDenseIndex(_id));
		break;

	case ColliderType::BOX:
		movedId = boxStorage->Remove(GetDenseIndex(_id));
		break;
	case ColliderType::CAPSULE:
		movedId = capsuleStorage->Remove(GetDenseIndex(_id));
	}

	if (!(movedId.GetIndex() == _id.GetIndex() && movedId.GetGeneration() == _id.GetGeneration()))
	{
		EditDenseIndex(movedId) = GetDenseIndex(_id);
	}

	// aabbも消す
	movedId = aabbStorage->Remove(GetAABBIndex(_id));
	if (!(movedId.GetIndex() == _id.GetIndex() && movedId.GetGeneration() == _id.GetGeneration()))
	{
		EditSlotMember<&ColliderSlot::aabbIndex>(_id) = GetAABBIndex(_id);
	}
	// 削除
	ReleaseID(_id);
}

ColliderType ColliderStorage::GetType(ColliderID _id) const
{
	return GetSlot(_id).type;
}

uint32_t ColliderStorage::GetAABBIndex(ColliderID _id) const
{
	return GetSlot(_id).aabbIndex;
}

PhysicsTransformID ColliderStorage::GetTransformID(ColliderID _id) const
{
	return GetSlot(_id).transformID;
}

std::vector<ColliderID>& ColliderStorage::GetColliderIDFromTransformID(PhysicsTransformID _id)
{
	return transformMap[_id];
}
