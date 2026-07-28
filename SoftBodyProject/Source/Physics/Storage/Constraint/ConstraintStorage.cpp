#include "ConstraintStorage.h"

ConstraintStorage::ConstraintStorage()
{
	pointConstraintStorage = std::make_unique<PointConstraintStorage>();
	distanceConstraintStorage = std::make_unique<DistanceConstraintStorage>();
}

ConstraintID ConstraintStorage::CreatePointConstraint(EntityID _entity, PhysicsTransformID _transformID,const Vector3& _localOffset)
{
	// ID作成
	ConstraintID id{ CreateID(ConstraintType::POINTS,pointConstraintStorage->CountConstraint(),_entity,_transformID)};

	// 実態を作る
	PointConstraint pointConstraint;
	pointConstraint.endPoints.emplace_back(_transformID, _localOffset);

	// 追加
	pointConstraintStorage->Add(id, pointConstraint);

	// ID返して終了
	return id;
}

ConstraintID ConstraintStorage::CreateDistanceConstraint(EntityID _entity, PhysicsTransformID _transformID, const Vector3& _localOffset, float _distance)
{
	// ID作成
	ConstraintID id{ CreateID(ConstraintType::DISTANCE,distanceConstraintStorage->CountConstraint(),_entity,_transformID)};
	
	// 実態を作る
	DistanceConstraint distanceConstraint;
	distanceConstraint.endPoints.emplace_back(_transformID, _localOffset);
	distanceConstraint.distance = _distance;

	// 追加
	distanceConstraintStorage->Add(id, distanceConstraint);

	// ID返して終了
	return id;
}

void ConstraintStorage::Destory(ConstraintID _id)
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
	}

	// 移動した奴の対応付けを戻す
	if (!(movedId.GetIndex() == _id.GetIndex() && movedId.GetGeneration() == _id.GetGeneration()))
	{
		EditDenseIndex(movedId) = GetDenseIndex(_id);
	}

	// 削除
	ReleaseID(_id);
}

ConstraintType ConstraintStorage::GetType(ConstraintID _id) const
{
	return GetSlot(_id).type;
}

PhysicsTransformID ConstraintStorage::GetTransformID(ConstraintID _id) const
{
	return GetSlot(_id).transformID;
}
