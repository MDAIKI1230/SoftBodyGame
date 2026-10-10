#pragma once

#include "Base/ConstraintBase.h"
#include "ConstraintTuning.h"

/*
	距離範囲に制限のある、ある点を線上に制限する拘束
	姿勢の制御はない
*/
struct LimitedPointOnLineConstraint : public ConstraintBase
{
	// 距離
	float distance{ 0.0f };

	// 柔らかさなどの調整用数値
	ConstraintTuning tuning;
};