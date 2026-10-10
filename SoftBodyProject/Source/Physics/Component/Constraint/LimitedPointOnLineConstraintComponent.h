#pragma once

#include "MDMath.h"

#include "ConstraintTuning.h"
#include "LimitedPointOnLineConstraint.h"

#include "Base/ConstraintComponentBase.h"

struct LimitedPointOnLineConstraintComponent :public ConstraintComponentBase<LimitedPointOnLineConstraint, false>
{
	friend class LimitedPointOnLineConstraintComponentStorage;
public:
	// コンストラクタ
	LimitedPointOnLineConstraintComponent(EntityID _entity);
	// コンストラクタ
	LimitedPointOnLineConstraintComponent(EntityID _entity, const Vector3& _localOffset);
	// コンストラクタ
	LimitedPointOnLineConstraintComponent(EntityID _entity, const Vector3& _localOffset, const Quaternion& _localRotation);

	// 距離取得
	float GetDistance() const;
	// 距離変更
	void SetDistance(float _distance);

	// 単一Tuning取得
	ConstraintTuning GetTuning() const;
	// 単一Tuning変更
	void SetTuning(const ConstraintTuning& _tuning);
};
