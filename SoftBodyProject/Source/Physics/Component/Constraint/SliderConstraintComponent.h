#pragma once

#include "MDMath.h"

#include "ConstraintTuning.h"
#include "SliderConstraint.h"

#include "Base/ConstraintComponentBase.h"

struct SliderConstraintComponent :public ConstraintComponentBase<SliderConstraint, false>
{
	friend class SliderConstraintComponentStorage;
public:
	// コンストラクタ
	SliderConstraintComponent(EntityID _entity);
	// コンストラクタ
	SliderConstraintComponent(EntityID _entity, const Vector3& _localOffset);
	// コンストラクタ
	SliderConstraintComponent(EntityID _entity, const Vector3& _localOffset, const Quaternion& _localRotation);

	// 単一Tuning取得
	ConstraintTuning GetTuning() const;
	// 単一Tuning変更
	void SetTuning(const ConstraintTuning& _tuning);
};
