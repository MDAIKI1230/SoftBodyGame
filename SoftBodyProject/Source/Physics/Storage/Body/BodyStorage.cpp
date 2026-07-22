#include "BodyStorage.h"

BodyStorage::BodyStorage()
{
	rigidBodyStorage = std::make_unique<RigidBodyStorage>();
	ropeStorage = std::make_unique<RopeStorage>();
	clothStorage = std::make_unique<ClothStorage>();
	softBodyStorage = std::make_unique<SoftBodyStorage>();
}

BodyID BodyStorage::CreateRigidBody(const EntityID& _entity, const PhysicsTransformID& _transformID)
{
	BodyID result{ GenerateBodyID(rigidBodyStorage->id.size(),BodyType::RIGID_BODY,_entity,_transformID) };

	rigidBodyStorage->CreateRigidBody(_entity, _transformID, result);

	// Map対応付け
	transformMap[_transformID] = result;

	return result;
}

BodyID BodyStorage::CreateRope(const EntityID& _entity, const PhysicsTransformID& _transformID)
{

	return BodyID{};
}

BodyID BodyStorage::CreateCloth(const EntityID& _entity, const PhysicsTransformID& _transformID)
{
	return BodyID{};
}
BodyID BodyStorage::CreateSoftBody(const EntityID& _entity, const PhysicsTransformID& _transformID)
{
	return BodyID{};
}

void BodyStorage::Destroy(const BodyID& _id)
{
	if (!IsAlive(_id))
	{
		return;
	}

	// swap-removeしたときの移動したID
	BodyID movedId;

	switch (slots[_id.index].type)
	{
	case BodyType::RIGID_BODY:
		movedId = rigidBodyStorage->Remove(slots[_id.index].denseIndex);
		break;
	case BodyType::ROPE:
		movedId = ropeStorage->Remove(slots[_id.index].denseIndex);
		break;
	case BodyType::CLOTH:
		movedId = clothStorage->Remove(slots[_id.index].denseIndex);
		break;
	case BodyType::SOFT_BODY:
		movedId = softBodyStorage->Remove(slots[_id.index].denseIndex);
		break;
	default:
		break;
	}

	// Mapから削除
	auto it = transformMap.find(slots[_id.index].transformID);
	if (it != transformMap.end())
	{
		transformMap.erase(it);
	}

	// 移動後のIDの修正
	slots[movedId.index].denseIndex = slots[_id.index].denseIndex;

	// フリーに追加
	freeSlots.push_back(_id.index);
	slots[_id.index].generation++;
}

bool BodyStorage::IsAlive(const BodyID& _id) const
{
	return slots[_id.index].alive && slots[_id.index].generation == _id.generation;
}

BodyType BodyStorage::GetType(const BodyID& _id) const
{
	return slots[_id.index].type;
}

uint32_t BodyStorage::GetDenseIndex(const BodyID& _id) const
{
	return slots[_id.index].denseIndex;
}

EntityID BodyStorage::GetOwnerEntity(const BodyID& _id) const
{
	return slots[_id.index].ownerEntity;
}

PhysicsTransformID BodyStorage::GetTransformID(const BodyID& _id) const
{
	return slots[_id.index].transformID;
}

bool BodyStorage::TryGetRigidBodyID(const PhysicsTransformID& _transformID, BodyID& _output)
{
	if (transformMap.contains(_transformID))
	{
		_output = transformMap[_transformID];
		return true;
	}

	return false;
}

BodyID BodyStorage::GenerateBodyID(size_t _denseIndex, const BodyType& _type, const EntityID& _ownerEntity, const PhysicsTransformID& _transformID)
{
	if (freeSlots.empty())
	{
		// --- フリーのスロットがないため新たにスロットを作成---

		// IDを作成(初代判定で1)
		BodyID result{ slots.size(),1 };
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

