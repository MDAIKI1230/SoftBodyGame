#include "ConstraintStorage.h"

ConstraintStorage::ConstraintStorage()
{
	pointConstraintStorage = std::make_unique<PointConstraintStorage>();
	distanceConstraintStorage = std::make_unique<DistanceConstraintStorage>();
	hingeConstraintStorage = std::make_unique<HingeConstraintStorage>();
	angleLimitPointConstraintStorage = std::make_unique<AngleLimitPointConstraintStorage>();
	angleLimitHingeConstraintStorage = std::make_unique<AngleLimitHingeConstraintStorage>();
	limitedBallJointConstraintStorage = std::make_unique<LimitedBallJointConstraintStorage>();
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

	// 対応表
	transformMap[_transformID].push_back(id);

	// ID返して終了
	return id;
}

ConstraintID ConstraintStorage::CreateDistanceConstraint(EntityID _entity, PhysicsTransformID _transformID, const Vector3& _localOffset)
{
	// ID作成
	ConstraintID id{ CreateID(ConstraintType::DISTANCE,distanceConstraintStorage->CountConstraint(),_entity,_transformID)};
	
	// 実態を作る
	DistanceConstraint distanceConstraint;
	distanceConstraint.endPoints.emplace_back(_transformID, _localOffset);

	// 追加
	distanceConstraintStorage->Add(id, distanceConstraint);

	// 対応表
	transformMap[_transformID].push_back(id);

	// ID返して終了
	return id;
}

// ヒンジ拘束作成関数
ConstraintID ConstraintStorage::CreateHingeConstraint(EntityID _entity, PhysicsTransformID _transformID, const Vector3& _localOffset, const Quaternion& _localRotation)
{
	// ID作成
	ConstraintID id{ CreateID(ConstraintType::HINGE,hingeConstraintStorage->CountConstraint(),_entity,_transformID) };

	// 実態を作る
	HingeConstraint hingeConstraint;
	hingeConstraint.ownerEndPoint = EndPointFrame{ _transformID,_localOffset,_localRotation };

	// 追加
	hingeConstraintStorage->Add(id, hingeConstraint);

	// 対応表
	transformMap[_transformID].push_back(id);

	// ID返して終了
	return id;
}

// 角度制限付き点拘束作成関数
ConstraintID ConstraintStorage::CreateAngleLimitPointConstraint(EntityID _entity, PhysicsTransformID _transformID, const Vector3& _localOffset, const Quaternion& _localRotation)
{
	// ID作成
	ConstraintID id{ CreateID(ConstraintType::ANGLE_LIMIT_POINT,angleLimitPointConstraintStorage->CountConstraint(),_entity,_transformID) };

	// 実態を作る
	AngleLimitPointConstraint angleLimitPointConstraint;
	angleLimitPointConstraint.ownerEndPoint = EndPointFrame{ _transformID, _localOffset, _localRotation };

	// 追加
	angleLimitPointConstraintStorage->Add(id, angleLimitPointConstraint);

	// 対応表
	transformMap[_transformID].push_back(id);

	// ID返して終了
	return id;
}

// 角度制限付きヒンジ拘束作成関数
ConstraintID ConstraintStorage::CreateAngleLimitHingeConstraint(
	EntityID _entity, PhysicsTransformID _transformID,
	const Vector3& _localOffset, const Quaternion& _localRotation)
{
	// ID作成
	ConstraintID id{ CreateID(ConstraintType::ANGLE_LIMIT_HINGE,angleLimitHingeConstraintStorage->CountConstraint(),_entity,_transformID) };

	// 実態を作る
	AngleLimitHingeConstraint angleLimitHingeConstraint;
	angleLimitHingeConstraint.ownerEndPoint = EndPointFrame{ _transformID ,_localOffset ,_localRotation };

	// 追加
	angleLimitHingeConstraintStorage->Add(id, angleLimitHingeConstraint);

	// 対応表
	transformMap[_transformID].push_back(id);

	// ID返して終了
	return id;
}

// SwingTwist拘束作成関数
ConstraintID ConstraintStorage::CreateLimitedBallJointConstraint(EntityID _entity, PhysicsTransformID _transformID, const Vector3& _localOffset, const Quaternion& _localRotation)
{
	// ID作成
	ConstraintID id{ CreateID(ConstraintType::LIMITED_BALL_JOINT,limitedBallJointConstraintStorage->CountConstraint(),_entity,_transformID) };

	// 実態を作る
	LimitedBallJointConstraint limitedBallJointConstraint;
	limitedBallJointConstraint.ownerEndPoint = EndPointFrame{ _transformID ,_localOffset ,_localRotation };

	// 追加
	limitedBallJointConstraintStorage->Add(id, limitedBallJointConstraint);

	// 対応表
	transformMap[_transformID].push_back(id);

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
		break;
	case ConstraintType::HINGE:
		movedId = hingeConstraintStorage->Remove(GetDenseIndex(_id));
		break;
	case ConstraintType::ANGLE_LIMIT_POINT:
		movedId = angleLimitPointConstraintStorage->Remove(GetDenseIndex(_id));
		break;
	case ConstraintType::ANGLE_LIMIT_HINGE:
		movedId = angleLimitHingeConstraintStorage->Remove(GetDenseIndex(_id));
		break;
	case ConstraintType::LIMITED_BALL_JOINT:
		movedId = limitedBallJointConstraintStorage->Remove(GetDenseIndex(_id));
		break;
	default:
		break;
	}

	// 対応表からも消す
	transformMap.erase(GetTransformID(_id));

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

// TransformIDから対応した拘束取得
bool ConstraintStorage::TryGetConstraintIDFromTransformID(PhysicsTransformID _id, std::vector<ConstraintID>& _output)
{
	if (transformMap.contains(_id))
	{
		_output = transformMap[_id];
		return true;
	}

	return false;
}
