#pragma once

#include <stdint.h>

#include "MDMath.h"

struct ContactConstraint
{
	uint32_t solverBodyAIndex;
	uint32_t solverBodyBIndex;

	Vector3 positionLocalA;
	Vector3 positionLocalB;

	Vector3 normal;
	float penetration;

	// 蓄積したλ
	float accumulatedLambda{ 0 };
	float accumulatedFrictionLambda{ 0 };
};
