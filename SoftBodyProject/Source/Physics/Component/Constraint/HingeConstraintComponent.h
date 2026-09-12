#pragma once

#include "MDMath.h"

#include "ConstraintTuning.h"

#include "Base/DirectionEndPointConstraintComponentBase.h"

struct HingeConstraintComponent :public DirectionEndPointConstraintComponentBase
{
	friend class HingeConstraintComponentStorage;
public:
	// コンストラクタ
	HingeConstraintComponent(EntityID _entity);
	// コンストラクタ
	HingeConstraintComponent(EntityID _entity, Vector3 _localOffset);
	// コンストラクタ
	HingeConstraintComponent(EntityID _entity, Vector3 _localOffset, Vector3 _localDirection);

	// 位置Tuning取得
	ConstraintTuning GetPositionTuning() const;
	// 位置Tuning変更
	void SetPositionTuning(const ConstraintTuning& _tuning);

	// 回転Tuning取得
	ConstraintTuning GetAngularTuning() const;
	// 回転Tuning変更
	void SetAngularTuning(const ConstraintTuning& _tuning);
};