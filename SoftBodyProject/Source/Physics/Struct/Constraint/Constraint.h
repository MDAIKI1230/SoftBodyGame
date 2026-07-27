#pragma once

#include "MDMath.h"

struct Constraint
{
	// ソルバボディA
	uint32_t solverBodyAIndex{ 0 };
	// ソルバボディB
	uint32_t solverBodyBIndex{ 0 };

	// ズレの数値(拘束条件Cの値と考えてOK)
	float error{ 0.0f };
	// Aの速度、Aの角速度、Bの速度、Bの角速度
	Vector3 jacobian[4];

	// 蓄積したλ
	float accumulatedLambda{ 0 };
	float accumulatedFrictionLambda{ 0 };
};
