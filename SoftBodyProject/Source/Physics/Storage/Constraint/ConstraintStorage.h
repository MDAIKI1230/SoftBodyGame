#pragma once

#include "StorageAccessorsMacros.h"

#include "ConstraintID.h"
#include "ConstraintSlot.h"

#include "PhysicsStorageBase.h"

#include "PointConstraintStorage.h"
#include "DistanceConstraintStorage.h"
#include "HingeConstraintStorage.h"
#include "AngleLimitPointConstraintStorage.h"
#include "AngleLimitHingeConstraintStorage.h"
#include "LimitedBallJointConstraintStorage.h"
#include "JointDriveConstraintStorage.h"

class ConstraintStorage :public PhysicsStorageBase<ConstraintID, ConstraintSlot>
{
	// 点拘束
	MD_OWNED_STORAGE_READ_ONLY_ACCESSORS(ConstraintID, ConstraintID, PointConstraintID, pointConstraintStorage, ID);
	MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(ConstraintID, PointConstraint, PointConstraint, pointConstraintStorage, Constraint);
	// 距離拘束
	MD_OWNED_STORAGE_READ_ONLY_ACCESSORS(ConstraintID, ConstraintID, DistanceConstraintID, distanceConstraintStorage, ID);
	MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(ConstraintID, DistanceConstraint, DistanceConstraint, distanceConstraintStorage, Constraint);
	// ヒンジ拘束
	MD_OWNED_STORAGE_READ_ONLY_ACCESSORS(ConstraintID, ConstraintID, HingeConstraintID, hingeConstraintStorage, ID);
	MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(ConstraintID, HingeConstraint, HingeConstraint, hingeConstraintStorage, Constraint);
	// 角度制限付き点拘束
	MD_OWNED_STORAGE_READ_ONLY_ACCESSORS(ConstraintID, ConstraintID, AngleLimitPointConstraintID, angleLimitPointConstraintStorage, ID);
	MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(ConstraintID, AngleLimitPointConstraint, AngleLimitPointConstraint, angleLimitPointConstraintStorage, Constraint);
	// 角度制限付きヒンジ拘束
	MD_OWNED_STORAGE_READ_ONLY_ACCESSORS(ConstraintID, ConstraintID, AngleLimitHingeConstraintID, angleLimitHingeConstraintStorage, ID);
	MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(ConstraintID, AngleLimitHingeConstraint, AngleLimitHingeConstraint, angleLimitHingeConstraintStorage, Constraint);
	// SwingTwist拘束
	MD_OWNED_STORAGE_READ_ONLY_ACCESSORS(ConstraintID, ConstraintID, LimitedBallJointConstraintID, limitedBallJointConstraintStorage, ID);
	MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(ConstraintID, LimitedBallJointConstraint, LimitedBallJointConstraint, limitedBallJointConstraintStorage, Constraint);
	// 関節駆動拘束
	MD_OWNED_STORAGE_READ_ONLY_ACCESSORS(ConstraintID, ConstraintID, JointDriveConstraintConstraintID, jointDriveConstraintStorage, ID);
	MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(ConstraintID, JointDriveConstraint, JointDriveConstraint, jointDriveConstraintStorage, Constraint);
public:
	// コンストラクタ
	ConstraintStorage();

	// 点拘束作成関数
	ConstraintID CreatePointConstraint(EntityID _entity, PhysicsTransformID _transformID, const Vector3& _localOffset);
	// 距離拘束作成関数
	ConstraintID CreateDistanceConstraint(EntityID _entity, PhysicsTransformID _transformID, const Vector3& _localOffset);
	// ヒンジ拘束作成関数
	ConstraintID CreateHingeConstraint(EntityID _entity, PhysicsTransformID _transformID, const Vector3& _localOffset, const Quaternion& _localRotation);
	// 角度制限付き点拘束作成関数
	ConstraintID CreateAngleLimitPointConstraint(EntityID _entity, PhysicsTransformID _transformID, const Vector3& _localOffset, const Quaternion& _localRotation);
	// 角度制限付きヒンジ拘束作成関数
	ConstraintID CreateAngleLimitHingeConstraint(
		EntityID _entity, PhysicsTransformID _transformID,
		const Vector3& _localOffset, const Quaternion& _localRotation);
	// SwingTwist拘束作成関数
	ConstraintID CreateLimitedBallJointConstraint(EntityID _entity, PhysicsTransformID _transformID, const Vector3& _localOffset, const Quaternion& _localRotation);
	// 関節駆動拘束作成関数
	ConstraintID CreateJointDriveConstraint(EntityID _entity, PhysicsTransformID _transformID, const Vector3& _localOffset, const Quaternion& _localRotation);

	// 破棄
	void Destory(ConstraintID _id);

	// 種類取得
	ConstraintType GetType(ConstraintID _id) const;
	// TransformID
	PhysicsTransformID GetTransformID(ConstraintID _id) const;

	// TransformIDから対応した拘束取得
	bool TryGetConstraintIDFromTransformID(PhysicsTransformID _id, std::vector<ConstraintID>& _output);
private:
	// --- 各種拘束ストレージ ---

	std::unique_ptr<PointConstraintStorage> pointConstraintStorage;
	std::unique_ptr<DistanceConstraintStorage> distanceConstraintStorage;
	std::unique_ptr<HingeConstraintStorage> hingeConstraintStorage;
	std::unique_ptr<AngleLimitPointConstraintStorage> angleLimitPointConstraintStorage;
	std::unique_ptr<AngleLimitHingeConstraintStorage> angleLimitHingeConstraintStorage;
	std::unique_ptr<LimitedBallJointConstraintStorage> limitedBallJointConstraintStorage;
	std::unique_ptr<JointDriveConstraintStorage> jointDriveConstraintStorage;

	// PhysicsTransformIDとの対応表
	std::unordered_map<PhysicsTransformID, std::vector<ConstraintID>> transformMap;
};
