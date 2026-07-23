#include "ConstraintStorage.h"

ConstraintStorage::ConstraintStorage()
{
	pointConstraintStorage = std::make_unique<PointConstraintStorage>();
	distanceConstraintStorage = std::make_unique<DistanceConstraintStorage>();
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

ConstraintID ConstraintStorage::CreateDistanceConstraint(EntityID& _entity, PhysicsTransformID _transformID, const Vector3& _localOffset, float _distance)
{
	// ID作成
	ConstraintID id{ GenerateConstraintID(ConstraintType::DISTANCE,distanceConstraintStorage->constraints.size(),_entity,_transformID) };
	
	// 実態を作る
	DistanceConstraint distanceConstraint;
	distanceConstraint.endPoints.emplace_back(_transformID, _localOffset);
	distanceConstraint.distance = _distance;

	// 追加
	distanceConstraintStorage->constraints.push_back(distanceConstraint);
	distanceConstraintStorage->id.push_back(id);

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
	case ConstraintType::DISTANCE:
		movedId = distanceConstraintStorage->Remove(GetDenseIndex(_id));
	default:
		break;
	}

	// 移動した奴の対応付けを戻す
	if (!(movedId.GetIndex() == _id.GetIndex() && movedId.GetGeneration() == _id.GetGeneration()))
	{
		slots[movedId.GetIndex()].denseIndex = slots[_id.GetIndex()].denseIndex;
	}

	// 削除
	slots[_id.GetIndex()].alive = false;
	slots[_id.GetIndex()].generation++;
	freeSlots.push_back(_id.GetIndex());
}

bool ConstraintStorage::IsAlive(ConstraintID& _id) const
{
	return slots[_id.GetIndex()].alive && slots[_id.GetIndex()].generation == _id.GetGeneration();
}

ConstraintType ConstraintStorage::GetType(ConstraintID& _id) const
{
	return slots[_id.GetIndex()].type;
}

uint32_t ConstraintStorage::GetDenseIndex(ConstraintID& _id) const
{
	return slots[_id.GetIndex()].denseIndex;
}

EntityID ConstraintStorage::GetOwnerEntity(ConstraintID& _id) const
{
	return slots[_id.GetIndex()].ownerEntity;
}

PhysicsTransformID ConstraintStorage::GetTransformID(ConstraintID& _id) const
{
	return slots[_id.GetIndex()].transformID;
}

ConstraintID ConstraintStorage::GenerateConstraintID(ConstraintType _type, uint32_t _denseIndex, EntityID _ownerEntity, PhysicsTransformID _transformID)
{
	if (freeSlots.empty())
	{
		// --- フリーのスロットがないため新たにスロットを作成---

		// IDを作成(初代判定で1)
		ConstraintID result{ static_cast<ConstraintID::Index>(slots.size()),1 };
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
