#pragma once

#include "MDMath.h"

#include "ConstraintTuning.h"
#include "PointOnLineConstraint.h"

#include "Base/ConstraintComponentBase.h"

struct PointOnLineConstraintComponent :public ConstraintComponentBase<PointOnLineConstraint, false>
{
	friend class PointOnLineConstraintComponentStorage;
public:
	// コンストラクタ
	PointOnLineConstraintComponent(EntityID _entity);
	// コンストラクタ
	PointOnLineConstraintComponent(EntityID _entity, const Vector3& _localOffset);
	// コンストラクタ
	PointOnLineConstraintComponent(EntityID _entity, const Vector3& _localOffset, const Quaternion& _localRotation);

	// 単一Tuning取得
	ConstraintTuning GetTuning() const;
	// 単一Tuning変更
	void SetTuning(const ConstraintTuning& _tuning);
};
