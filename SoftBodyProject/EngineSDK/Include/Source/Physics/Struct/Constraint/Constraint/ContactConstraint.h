#pragma once

#include <stdint.h>

#include "MDMath.h"

struct ContactConstraint
{
	uint32_t solverBodyAIndex{ 0 };
	uint32_t solverBodyBIndex{ 0 };

	Vector3 positionLocalA;
	Vector3 positionLocalB;

	Vector3 normal;
	float penetration{ 0.0f };

	// 蓄積したλ
	float accumulatedLambda{ 0 };
	float accumulatedFrictionLambda{ 0 };

	// 速度摩擦用の累積インパルス：接線2方向
	float accumulatedVelocityFrictionLambda[2]{ 0.0f, 0.0f };
};
