#pragma once

#include "GenerationalID.h"

struct PhysicsTransformTag;

using PhysicsTransformID = GenerationalID<PhysicsTransformTag>;

template<>
struct std::hash<PhysicsTransformID>
{
	size_t operator()(const PhysicsTransformID& p) const
	{
		return std::hash<size_t>{}(p.GetIndex())
			^ (std::hash<size_t>{}(p.GetGeneration()) << 1);
	}
};
