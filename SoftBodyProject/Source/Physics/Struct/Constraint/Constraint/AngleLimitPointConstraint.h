#pragma once

#include "Base/ConstraintBase.h"
#include "ConstraintTuning.h"

struct AngleLimitPointConstraint : public ConstraintBase
{
	// 制限角度
	float angleMax{ 0.0f };
	float angleMin{ 0.0f };

	// 柔らかさなどの調整用数値
	ConstraintTuning tuning;
};