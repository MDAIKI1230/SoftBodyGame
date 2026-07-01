#pragma once

#include <stdint.h>

#include "MDMath.h"

struct ContactConstraint
{
	uint32_t solverBodyAIndex;
	uint32_t solverBodyBIndex;

	Vector3 position;
	Vector3 normal;
	float penetration;

	// 蓄積したλ
	float accumulatedLambda{ 0 };
};
