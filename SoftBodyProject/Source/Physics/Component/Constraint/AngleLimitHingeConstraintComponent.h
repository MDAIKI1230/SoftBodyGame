#pragma once

#include "MDMath.h"

#include "EntityID.h"
#include "ConstraintTuning.h"

#include "Base/ConstraintComponentBase.h"

struct AngleLimitHingeConstraintComponent :public ConstraintComponentBase
{
public:
	// コンストラクタ
	AngleLimitHingeConstraintComponent(EntityID _entity);
	// コンストラクタ
	AngleLimitHingeConstraintComponent(EntityID _entity, const Vector3& _localOffset);
	// コンストラクタ
	AngleLimitHingeConstraintComponent(EntityID _entity, const Vector3& _localOffset, const Quaternion& _localRotation);

	// 最小角度取得
	float GetAngleMin() const;
	// 最小角度変更
	void SetAngleMin(float _angleMin);

	// 最大角度取得
	float GetAngleMax() const;
	// 最大角度変更
	void SetAngleMax(float _angleMax);

	// 角度範囲変更
	void SetAngleRange(float _angleMin, float _angleMax);

	// 位置Tuning取得
	ConstraintTuning GetPositionTuning() const;
	// 位置Tuning変更
	void SetPositionTuning(const ConstraintTuning& _tuning);

	// 回転Tuning取得
	ConstraintTuning GetAngularTuning() const;
	// 回転Tuning変更
	void SetAngularTuning(const ConstraintTuning& _tuning);
};