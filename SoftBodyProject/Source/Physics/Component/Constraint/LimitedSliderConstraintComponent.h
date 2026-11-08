#pragma once

#include "MDMath.h"

#include "ConstraintTuning.h"
#include "LimitedSliderConstraint.h"

#include "Base/ConstraintComponentBase.h"

struct LimitedSliderConstraintComponent :public ConstraintComponentBase<LimitedSliderConstraint, false>
{
	friend class LimitedSliderConstraintComponentStorage;
public:
	// コンストラクタ
	LimitedSliderConstraintComponent(EntityID _entity);
	// コンストラクタ
	LimitedSliderConstraintComponent(EntityID _entity, const Vector3& _localOffset);
	// コンストラクタ
	LimitedSliderConstraintComponent(EntityID _entity, const Vector3& _localOffset, const Quaternion& _localRotation);

	// 距離取得
	float GetDistance() const;
	// 距離変更
	void SetDistance(float _distance);

	// 単一Tuning取得
	ConstraintTuning GetTuning() const;
	// 単一Tuning変更
	void SetTuning(const ConstraintTuning& _tuning);
};
