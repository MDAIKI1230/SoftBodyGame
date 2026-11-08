#pragma once

#include "Base/ConstraintBase.h"
#include "ConstraintTuning.h"

struct PointConstraint :public ConstraintBase
{
	// 柔らかさなどの調整用数値
	ConstraintTuning tuning;
};
