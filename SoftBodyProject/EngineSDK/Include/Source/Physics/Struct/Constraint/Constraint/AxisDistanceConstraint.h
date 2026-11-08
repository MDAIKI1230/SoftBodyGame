#pragma once

#include "Base/ConstraintOneBase.h"
#include "ConstraintTuning.h"

/*
	自身側EndPointのローカルY軸に沿った距離を保つ拘束
	軸に垂直な方向の位置と、相対姿勢は制限しない
*/
struct AxisDistanceConstraint :public ConstraintOneBase
{
	// 相手側から自身側への、軸方向の符号付き目標距離
	float distance{ 0.0f };

	// 柔らかさなどの調整用数値
	ConstraintTuning tuning;
};