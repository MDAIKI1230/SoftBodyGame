#include "BodyStorage.h"

// 質量の代入
void BodyStorage::SetRigidBodyMass(BodyID _id, float _mass)
{
	rigidBodyStorage->SetMass(GetDenseIndex(_id), _mass);
}

// 計算が完了したときに呼ぶ関数
void BodyStorage::LocalInertiaCalcSucces(BodyID _id)
{
	rigidBodyStorage->LocalInertiaCalcSucces(GetDenseIndex(_id));
}

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
	BodyID result{ GenerateBodyID(rigidBodyStorage->CountID(),BodyType::RIGID_BODY,_entity,_transformID) };

	rigidBodyStorage->Create(result);

	// Map対応付け
	transformMap[_transformID] = result;

	return result;
}
// Rope作成
BodyID BodyStorage::CreateRope(EntityID _entity, PhysicsTransformID _transformID, const RopeUpdateInfo& _info)
{
	BodyID result{ GenerateBodyID(ropeStorage->CountID(),BodyType::ROPE,_entity,_transformID)};

	ropeStorage->CreateRope(result, _info);

	return result;
}
// Cloth作成
BodyID BodyStorage::CreateCloth(EntityID _entity, PhysicsTransformID _transformID, const ClothUpdateInfo& _info)
{
	BodyID result{ GenerateBodyID(clothStorage->CountID(),BodyType::CLOTH,_entity,_transformID)};

	clothStorage->Create(result, _info);

	return result;
}
// SoftBody作成
BodyID BodyStorage::CreateSoftBody(EntityID _entity, PhysicsTransformID _transformID, const SoftBodyUpdateInfo& _info)
{
	BodyID result{ GenerateBodyID(softBodyStorage->CountID(),BodyType::SOFT_BODY,_entity,_transformID) };

	softBodyStorage->Create(result, _info);
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
BodyID BodyStorage::GenerateBodyID(uint32_t _denseIndex, BodyType _type, EntityID _ownerEntity, PhysicsTransformID _transformID)
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

