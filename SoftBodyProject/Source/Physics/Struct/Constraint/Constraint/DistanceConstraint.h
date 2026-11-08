#pragma once

#include "Base/ConstraintBase.h"
#include "ConstraintTuning.h"

struct DistanceConstraint :public ConstraintBase
{
	// 距離
	float distance{ 0.0f };

	// 柔らかさなどの調整用数値
	ConstraintTuning tuning;
};
