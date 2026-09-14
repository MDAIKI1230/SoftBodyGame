#pragma once

#include "MDMath.h"

#include "ConstraintTuning.h"

#include "Base/ConstraintComponentBase.h"

struct AngleLimitPointConstraintComponent :public ConstraintComponentBase
{
	friend class AngleLimitPointConstraintComponentStorage;
public:
	// コンストラクタ
	AngleLimitPointConstraintComponent(EntityID _entity);
	// コンストラクタ
	AngleLimitPointConstraintComponent(EntityID _entity, const Vector3& _localOffset);
	// コンストラクタ
	AngleLimitPointConstraintComponent(EntityID _entity, const Vector3& _localOffset, const Quaternion& _localRotation);

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

	// Tuning取得
	ConstraintTuning GetTuning() const;
	// Tuning変更
	void SetTuning(const ConstraintTuning& _tuning);
};