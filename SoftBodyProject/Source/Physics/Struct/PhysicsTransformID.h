#pragma once

#include <stdint.h>

struct PhysicsTransformID
{
	uint32_t index{ 0 };
	unsigned char generation{ 0 };

	bool operator==(const PhysicsTransformID& other) const
	{
		return index == other.index && generation == other.generation;
	}
};

template<>
struct std::hash<PhysicsTransformID>
{
	size_t operator()(const PhysicsTransformID& p) const
	{
		return std::hash<size_t>{}(p.index)
			^ (std::hash<size_t>{}(p.generation) << 1);
	}
};
