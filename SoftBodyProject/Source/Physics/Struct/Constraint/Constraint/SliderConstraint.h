#pragma once

#include "Base/ConstraintBase.h"
#include "ConstraintTuning.h"

/*
	スライダー拘束
*/
struct SliderConstraint :public ConstraintBase
{
	// 柔らかさなどの調整用数値
	ConstraintTuning tuning;
};