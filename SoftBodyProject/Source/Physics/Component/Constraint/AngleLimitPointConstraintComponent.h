#pragma once

#include "MDMath.h"

#include "ConstraintTuning.h"

#include "Base/DirectionEndPointConstraintComponentBase.h"

struct AngleLimitPointConstraintComponent :public DirectionEndPointConstraintComponentBase
{
	friend class AngleLimitPointConstraintComponentStorage;
public:
	// コンストラクタ
	AngleLimitPointConstraintComponent(EntityID _entity);
	// コンストラクタ
	AngleLimitPointConstraintComponent(EntityID _entity, Vector3 _localOffset);
	// コンストラクタ
	AngleLimitPointConstraintComponent(EntityID _entity, float _angleMin, float _angleMax);
	// コンストラクタ
	AngleLimitPointConstraintComponent(EntityID _entity, Vector3 _localOffset, Vector3 _localDirection);
	// コンストラクタ
	AngleLimitPointConstraintComponent(EntityID _entity, Vector3 _localOffset, Vector3 _localDirection, float _angleMin, float _angleMax);

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