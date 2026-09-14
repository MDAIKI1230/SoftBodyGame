#pragma once

#include "MDMath.h"

#include "ConstraintTuning.h"

#include "Base/ConstraintComponentBase.h"

struct HingeConstraintComponent :public ConstraintComponentBase
{
	friend class HingeConstraintComponentStorage;
public:
	// コンストラクタ
	HingeConstraintComponent(EntityID _entity);

	// 位置Tuning取得
	ConstraintTuning GetPositionTuning() const;
	// 位置Tuning変更
	void SetPositionTuning(const ConstraintTuning& _tuning);

	// 回転Tuning取得
	ConstraintTuning GetAngularTuning() const;
	// 回転Tuning変更
	void SetAngularTuning(const ConstraintTuning& _tuning);
};