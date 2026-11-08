#pragma once

#include "Base/ConstraintBase.h"
#include "ConstraintTuning.h"

/*
	距離制限のある、スライダー拘束
*/
struct LimitedSliderConstraint :public ConstraintBase
{
	// 距離
	float distance{ 0.0f };

	// 柔らかさなどの調整用数値
	ConstraintTuning tuning;
};