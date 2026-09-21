#pragma once

#include "MDMath.h"

struct HandIK
{
	Quaternion targetRotation;
	float rotationWeight{ 1.0f };
};