#include "BodyStorage.h"

// コンストラクタ
BodyStorage::BodyStorage()
{
	rigidBodyStorage = std::make_unique<RigidBodyStorage>();
	ropeStorage = std::make_unique<RopeStorage>();
	clothStorage = std::make_unique<ClothStorage>();
	softBodyStorage = std::make_unique<SoftBodyStorage>();
}

// RigidBody作成
BodyID BodyStorage::CreateRigidBody(EntityID _entity, PhysicsTransformID _transformID)
{
	BodyID result{ GenerateBodyID(rigidBodyStorage->id.size(),BodyType::RIGID_BODY,_entity,_transformID) };

	rigidBodyStorage->CreateRigidBody(_entity, _transformID, result);

	// Map対応付け
	transformMap[_transformID] = result;

	return result;
}
// Rope作成
BodyID BodyStorage::CreateRope(EntityID _entity, PhysicsTransformID _transformID, const RopeUpdateInfo& _info)
{
	BodyID result{ GenerateBodyID(ropeStorage->id.size(),BodyType::ROPE,_entity,_transformID) };

	ropeStorage->id.push_back(result);
	ropeStorage->meta.emplace_back();
	ropeStorage->length.emplace_back(_info.length);
	ropeStorage->segmentCount.emplace_back(_info.segmentCount);

	return result;
}
// Cloth作成
BodyID BodyStorage::CreateCloth(EntityID _entity, PhysicsTransformID _transformID, const ClothUpdateInfo& _info)
{
	BodyID result{ GenerateBodyID(clothStorage->id.size(),BodyType::CLOTH,_entity,_transformID) };

	clothStorage->id.push_back(result);
	clothStorage->meta.emplace_back();
	clothStorage->width.emplace_back(_info.width);
	clothStorage->height.emplace_back(_info.height);
	clothStorage->rowCount.emplace_back(_info.rowCount);
	clothStorage->columnCount.emplace_back(_info.columnCount);

	return result;
}
// SoftBody作成
BodyID BodyStorage::CreateSoftBody(EntityID _entity, PhysicsTransformID _transformID, const SoftBodyUpdateInfo& _info)
{
	BodyID result{ GenerateBodyID(softBodyStorage->id.size(),BodyType::SOFT_BODY,_entity,_transformID) };

	softBodyStorage->id.push_back(result);
	softBodyStorage->meta.emplace_back();
	softBodyStorage->width.emplace_back(_info.width);
	softBodyStorage->height.emplace_back(_info.height);
	softBodyStorage->depth.emplace_back(_info.depth);
	softBodyStorage->segmentCountX.emplace_back(_info.segmentCountX);
	softBodyStorage->segmentCountX.emplace_back(_info.segmentCountY);
	softBodyStorage->segmentCountX.emplace_back(_info.segmentCountZ);

	return result;
}

// 破棄
void BodyStorage::Destroy(BodyID _id)
{
	if (!IsAlive(_id))
	{
		return;
	}

	// swap-removeしたときの移動したID
	BodyID movedId;

	switch (slots[_id.GetIndex()].type)
	{
	case BodyType::RIGID_BODY:
		movedId = rigidBodyStorage->Remove(slots[_id.GetIndex()].denseIndex);
		break;
	case BodyType::ROPE:
		movedId = ropeStorage->Remove(slots[_id.GetIndex()].denseIndex);
		break;
	case BodyType::CLOTH:
		movedId = clothStorage->Remove(slots[_id.GetIndex()].denseIndex);
		break;
	case BodyType::SOFT_BODY:
		movedId = softBodyStorage->Remove(slots[_id.GetIndex()].denseIndex);
		break;
	default:
		break;
	}

	// Mapから削除
	auto it = transformMap.find(slots[_id.GetIndex()].transformID);
	if (it != transformMap.end())
	{
		transformMap.erase(it);
	}

	// 移動後のIDの修正
	slots[movedId.GetIndex()].denseIndex = slots[_id.GetIndex()].denseIndex;

	// フリーに追加
	freeSlots.push_back(_id.GetIndex());
	slots[_id.GetIndex()].generation++;
}

// 生存確認
bool BodyStorage::IsAlive(BodyID _id) const
{
	return slots[_id.GetIndex()].alive && slots[_id.GetIndex()].generation == _id.GetGeneration();
}
// 種類取得
BodyType BodyStorage::GetType(BodyID _id) const
{
	return slots[_id.GetIndex()].type;
}
// 実データのインデックス
uint32_t BodyStorage::GetDenseIndex(BodyID _id) const
{
	return slots[_id.GetIndex()].denseIndex;
}
// 持ってるEntity
EntityID BodyStorage::GetOwnerEntity(BodyID _id) const
{
	return slots[_id.GetIndex()].ownerEntity;
}
// 対応Transform
PhysicsTransformID BodyStorage::GetTransformID(BodyID _id) const
{
	return slots[_id.GetIndex()].transformID;
}
// RigidBodyのBodyID取得
bool BodyStorage::TryGetRigidBodyID(PhysicsTransformID _transformID, BodyID& _output)
{
	if (transformMap.contains(_transformID))
	{
		_output = transformMap[_transformID];
		return true;
	}

	return false;
}

// TransformIDと紐づくBodyIDがあるか否か
bool BodyStorage::Has(PhysicsTransformID _transformID) const
{
	return transformMap.contains(_transformID);
}

// 一意なID発行関数
BodyID BodyStorage::GenerateBodyID(size_t _denseIndex, BodyType _type, EntityID _ownerEntity, PhysicsTransformID _transformID)
{
	if (freeSlots.empty())
	{
		// --- フリーのスロットがないため新たにスロットを作成---

		// IDを作成(初代判定で1)
		BodyID result{ static_cast<BodyID::Index>(slots.size()),1 };
		// Slotを増設
		slots.emplace_back(_denseIndex, _type, _ownerEntity, _transformID);

		return result;
	}
	else
	{
		// フリーのスロットがあるためそれを使用

		// 最後を取る
		uint32_t index{ freeSlots.back() };
		freeSlots.pop_back();

		// 世代は削除時に加算済み
		slots[index].alive = true;
		slots[index].denseIndex = _denseIndex;
		slots[index].type = _type;
		slots[index].ownerEntity = _ownerEntity;
		slots[index].transformID = _transformID;

		// IDを作成(初代判定で1)
		return BodyID{ (uint32_t)(index),slots[index].generation };
	}
}

