#include "ConstraintStorage.h"

ConstraintStorage::ConstraintStorage()
{
	pointConstraintStorage = std::make_unique<PointConstraintStorage>();
}

ConstraintID ConstraintStorage::CreatePointConstraint(EntityID& _entity, PhysicsTransformID _transformID,const Vector3& _localOffset)
{
	// ID作成
	ConstraintID id{ GenerateConstraintID(ConstraintType::POINTS,pointConstraintStorage->constraints.size(),_entity,_transformID) };

	// 実態を作る
	PointConstraint pointConstraint;
	pointConstraint.endPoints.emplace_back(_transformID, _localOffset);

	// 追加
	pointConstraintStorage->constraints.push_back(pointConstraint);
	pointConstraintStorage->id.push_back(id);

	// ID返して終了
	return id;
}

void ConstraintStorage::Destory(ConstraintID& _id)
{
	// 生存チェック
	if (!IsAlive(_id))
	{
		return;
	}

	// 移動インデックス
	ConstraintID movedId{};

	switch (GetType(_id))
	{
	case  ConstraintType::POINTS:
		movedId = pointConstraintStorage->Remove(GetDenseIndex(_id));
		break;
	default:
		break;
	}

	// 移動した奴の対応付けを戻す
	if (!(movedId.index == _id.index && movedId.generation == _id.generation))
	{
		slots[movedId.index].denseIndex = slots[_id.index].denseIndex;
	}

	// 削除
	slots[_id.index].alive = false;
	slots[_id.index].generation++;
	freeSlots.push_back(_id.index);
}

bool ConstraintStorage::IsAlive(ConstraintID& _id) const
{
	return slots[_id.index].alive && slots[_id.index].generation == _id.generation;
}

ConstraintType ConstraintStorage::GetType(ConstraintID& _id) const
{
	return slots[_id.index].type;
}

uint32_t ConstraintStorage::GetDenseIndex(ConstraintID& _id) const
{
	return slots[_id.index].denseIndex;
}

EntityID ConstraintStorage::GetOwnerEntity(ConstraintID& _id) const
{
	return slots[_id.index].ownerEntity;
}

PhysicsTransformID ConstraintStorage::GetTransformID(ConstraintID& _id) const
{
	return slots[_id.index].transformID;
}

ConstraintID ConstraintStorage::GenerateConstraintID(ConstraintType _type, uint32_t _denseIndex, EntityID _ownerEntity, PhysicsTransformID _transformID)
{
	if (freeSlots.empty())
	{
		// --- フリーのスロットがないため新たにスロットを作成---

		// IDを作成(初代判定で1)
		ConstraintID result{ slots.size(),1 };
		// Slotを増設
		slots.emplace_back(_type, _denseIndex, _ownerEntity, _transformID);

		return result;
	}
	else
	{
		// --- フリーのスロットがあるためそれを使用 ---

		// 最後を取る
		uint32_t index{ freeSlots.back() };
		freeSlots.pop_back();

		// 世代は削除時に加算済み
		slots[index].alive = true;
		slots[index].type = _type;
		slots[index].denseIndex = _denseIndex;
		slots[index].ownerEntity = _ownerEntity;
		slots[index].transformID = _transformID;

		// IDを作成(初代判定で1)
		return ConstraintID{ index,slots[index].generation };
	}
}
