#pragma once

#include "Base/ConstraintBase.h"
#include "ConstraintTuning.h"

/*
	ある点をライン上に制限する拘束
	姿勢の制御はない
*/
struct PointOnLineConstraint :public ConstraintBase
{
	// 柔らかさなどの調整用数値
	ConstraintTuning tuning;
};