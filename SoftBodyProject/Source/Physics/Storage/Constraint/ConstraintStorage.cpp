#include "AssertMacros.h"

#include "ConstraintStorage.h"

ConstraintStorage::ConstraintStorage()
{
	pointConstraintStorage = std::make_unique<ConstraintStorageBase<PointConstraint>>();
	distanceConstraintStorage = std::make_unique<ConstraintStorageBase<DistanceConstraint>> ();
	hingeConstraintStorage = std::make_unique<ConstraintStorageBase<HingeConstraint>> ();
	angleLimitPointConstraintStorage = std::make_unique<ConstraintStorageBase<AngleLimitPointConstraint>> ();
	angleLimitHingeConstraintStorage = std::make_unique<ConstraintStorageBase<AngleLimitHingeConstraint>> ();
	limitedBallJointConstraintStorage = std::make_unique<ConstraintStorageBase<LimitedBallJointConstraint>> ();
	jointDriveConstraintStorage = std::make_unique<ConstraintStorageBase<JointDriveConstraint>> ();
	pointOnLineConstraintStorage = std::make_unique<ConstraintStorageBase<PointOnLineConstraint>>();
	sliderConstraintStorage = std::make_unique<ConstraintStorageBase<SliderConstraint>>();
	limitedPointOnLineConstraintStorage = std::make_unique<ConstraintStorageBase<LimitedPointOnLineConstraint>>();
	limitedSliderConstraintStorage = std::make_unique<ConstraintStorageBase<LimitedSliderConstraint>>();
}

// 種類がわからない時用のエンドポイント取得関数
const EndPointFrame& ConstraintStorage::GetEndPoint(ConstraintID _id)
{
	switch (GetType(_id))
	{
		CONSTRAINT_LIST(GET_ENDPOINT_CASE);
	default:
		break;
	}

	MD_UNREACHABLE("そんな拘束はありませーん");
}
// 種類がわからない時用の対応エンドポイント取得関数
std::span<const EndPointFrame> ConstraintStorage::GetOtherEndPoints(ConstraintID _id)
{
	switch (GetType(_id))
	{
		CONSTRAINT_LIST(GET_OTHERENDPOINTS_CASE);
	default:
		break;
	}

	MD_UNREACHABLE("そんな拘束はありませーん");
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
	case ConstraintType::JOINT_DRIVE:
		movedId = jointDriveConstraintStorage->Remove(GetDenseIndex(_id));
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
