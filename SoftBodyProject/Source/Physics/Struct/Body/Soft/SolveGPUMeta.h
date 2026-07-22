#pragma once

#include <cstdint>

struct SolveGPUMeta
{
	uint32_t particleDataStart;
	uint32_t particleDataCount;
	uint32_t constraintDataStart;
	uint32_t constraintDataCount;
};
