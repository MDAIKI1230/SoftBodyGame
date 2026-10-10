#pragma once

#include "StorageAccessorsMacros.h"

#include "ConstraintID.h"
#include "ConstraintSlot.h"

#include "PhysicsStorageBase.h"

#include "PointConstraint.h"
#include "DistanceConstraint.h"
#include "HingeConstraint.h"
#include "AngleLimitPointConstraint.h"
#include "AngleLimitHingeConstraint.h"
#include "LimitedBallJointConstraint.h"
#include "JointDriveConstraint.h"
#include "PointOnLineConstraint.h"
#include "SliderConstraint.h"
#include "LimitedPointOnLineConstraint.h"
#include "LimitedSliderConstraint.h"

#include "Base/ConstraintStorageBase.h"

#define CONSTRAINT_SCCESSORS(ConstraintType, EnumType, MemberName)\
private:\
std::unique_ptr<ConstraintStorageBase<ConstraintType>> MemberName;\
MD_OWNED_STORAGE_READ_ONLY_ACCESSORS(ConstraintID, ConstraintID, ConstraintType##ID, MemberName, ID);\
MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(ConstraintID, ConstraintType, ConstraintType, MemberName, Constraint);\
template<class T>\
requires std::same_as<T, ConstraintType>\
ConstraintID Create(EntityID _entity, PhysicsTransformID _transformID, const Vector3& _localOffset, const Quaternion& _localRotation = Quaternion::IDENTITY)\
{\
	ConstraintID id{ CreateID(EnumType,MemberName->CountConstraint(),_entity,_transformID) };\
	\
	ConstraintType constraint;\
	constraint.ownerEndPoint = EndPointFrame{ _transformID ,_localOffset ,_localRotation };\
	\
	MemberName->Add(id, constraint);\
	\
	transformMap[_transformID].push_back(id);\
	\
	return id;\
}

class ConstraintStorage :public PhysicsStorageBase<ConstraintID, ConstraintSlot>
{
	// 点拘束
	CONSTRAINT_SCCESSORS(PointConstraint, ConstraintType::POINTS, pointConstraintStorage);
	// 距離拘束
	CONSTRAINT_SCCESSORS(DistanceConstraint, ConstraintType::DISTANCE, distanceConstraintStorage);
	// ヒンジ拘束
	CONSTRAINT_SCCESSORS(HingeConstraint, ConstraintType::HINGE, hingeConstraintStorage);
	// 角度制限付き点拘束
	CONSTRAINT_SCCESSORS(AngleLimitPointConstraint, ConstraintType::ANGLE_LIMIT_POINT, angleLimitPointConstraintStorage);
	// 角度制限付きヒンジ拘束
	CONSTRAINT_SCCESSORS(AngleLimitHingeConstraint, ConstraintType::ANGLE_LIMIT_HINGE, angleLimitHingeConstraintStorage);
	// SwingTwist拘束
	CONSTRAINT_SCCESSORS(LimitedBallJointConstraint, ConstraintType::LIMITED_BALL_JOINT, limitedBallJointConstraintStorage);
	// 関節駆動拘束
	CONSTRAINT_SCCESSORS(JointDriveConstraint, ConstraintType::JOINT_DRIVE , jointDriveConstraintStorage);
	// ある点を線上に制限する拘束
	CONSTRAINT_SCCESSORS(PointOnLineConstraint, ConstraintType::POINT_ON_LINE, pointOnLineConstraintStorage);
	// スライダー拘束
	CONSTRAINT_SCCESSORS(SliderConstraint, ConstraintType::SLIDER, sliderConstraintStorage);
	// 距離制限のある、点を線上に制限する拘束
	CONSTRAINT_SCCESSORS(LimitedPointOnLineConstraint, ConstraintType::LIMITED_POINT_ON_LINE, limitedPointOnLineConstraintStorage);
	// 距離制限のある、スライダー拘束
	CONSTRAINT_SCCESSORS(LimitedSliderConstraint, ConstraintType::LIMITED_SLIDER, limitedSliderConstraintStorage);
public:
	// コンストラクタ
	ConstraintStorage();

	// 破棄
	void Destory(ConstraintID _id);

	// 種類取得
	ConstraintType GetType(ConstraintID _id) const;
	// TransformID
	PhysicsTransformID GetTransformID(ConstraintID _id) const;

	// TransformIDから対応した拘束取得
	bool TryGetConstraintIDFromTransformID(PhysicsTransformID _id, std::vector<ConstraintID>& _output);
private:
	// PhysicsTransformIDとの対応表
	std::unordered_map<PhysicsTransformID, std::vector<ConstraintID>> transformMap;
};
