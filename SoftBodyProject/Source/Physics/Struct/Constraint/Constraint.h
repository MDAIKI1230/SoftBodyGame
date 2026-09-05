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

	// 目標速度
	float targetVelocity{ 0.0f };
	// ERPから作られるバイアス
	float bias{ 0.0f };
	// 柔らかさ
	float softness{ 0.0f };
	// 加えれる力の最小値
	float minLambda{ -FLT_MAX };
	// 加えれる力の最大値
	float maxLambda{ FLT_MAX };
};
