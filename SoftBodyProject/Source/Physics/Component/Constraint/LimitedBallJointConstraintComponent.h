#pragma once

#include "EntityID.h"

#include "ConstraintTuning.h"

#include "Base/ConstraintComponentBase.h"

struct LimitedBallJointConstraintComponent :public ConstraintComponentBase
{
	friend class LimitedBallJointConstraintComponentStorage;
public:
	// コンストラクタ
	LimitedBallJointConstraintComponent(EntityID _entity);

	// Swing角度取得
	float GetSwingAngle();
	// Swing角度変更
	void SetSwingAngle(float _angle);

	// Twist最大角度取得
	float GetTwistAngleMax();
	// Twist最大角度変更
	void SetTwistAngleMax(float _angle);
	// Twist最小角度取得
	float GetTwistAngleMin();
	// Twist最小角度変更
	void SetTwistAngleMin(float _angle);

	// 位置Tuning取得
	ConstraintTuning GetPositionTuning() const;
	// 位置Tuning変更
	void SetPositionTuning(const ConstraintTuning& _tuning);

	// 回転Tuning取得
	ConstraintTuning GetAngularTuning() const;
	// 回転Tuning変更
	void SetAngularTuning(const ConstraintTuning& _tuning);
};