#pragma once

#include "MDMath.h"

#include "EntityID.h"
#include "ConstraintID.h"

#include "ConstraintTuning.h"

struct LimitedBallJointConstraintComponent
{
	friend class LimitedBallJointConstraintComponentStorage;
public:
	// コンストラクタ
	LimitedBallJointConstraintComponent(EntityID _entity);
	// コンストラクタ
	LimitedBallJointConstraintComponent(EntityID _entity, Vector3 _localOffset);
	// コンストラクタ
	LimitedBallJointConstraintComponent(EntityID _entity, Vector3 _localOffset, Vector3 _localDirection);

	// 対応点追加
	void AddEndPoint(EntityID _entityID, const Vector3& _localOffset, const Vector3& _localDirection);
	// 対応点削除
	void RemoveEndPoint(EntityID _entityID);

	// Swing角度取得
	float GetSwingAngle(ConstraintID _id);
	// Swing角度変更
	void SetSwingAngle(ConstraintID _id, float _angle);
	// Twist角度取得
	float GetTwistAngle(ConstraintID _id);
	// Twist角度変更
	void SetTwistAngle(ConstraintID _id, float _angle);

	// 位置Tuning取得
	ConstraintTuning GetPositionTuning() const;
	// 位置Tuning変更
	void SetPositionTuning(const ConstraintTuning& _tuning);

	// 回転Tuning取得
	ConstraintTuning GetAngularTuning() const;
	// 回転Tuning変更
	void SetAngularTuning(const ConstraintTuning& _tuning);
};