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
	// Twist角度取得
	float GetTwistAngle();
	// Twist角度変更
	void SetTwistAngle(float _angle);

	// 位置Tuning取得
	ConstraintTuning GetPositionTuning() const;
	// 位置Tuning変更
	void SetPositionTuning(const ConstraintTuning& _tuning);

	// 回転Tuning取得
	ConstraintTuning GetAngularTuning() const;
	// 回転Tuning変更
	void SetAngularTuning(const ConstraintTuning& _tuning);
};