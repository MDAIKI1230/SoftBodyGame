#pragma once

#include <stdint.h>

struct PhysicsTransformID
{
	uint32_t index{ 0 };
	unsigned char generation{ 0 };
};
