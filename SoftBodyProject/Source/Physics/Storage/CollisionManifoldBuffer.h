#pragma once

#include <vector>

#include "Manifold.h"

class CollisionManifoldBuffer
{
public:
	void Clear()
	{
		manifolds.clear();
	}
public:
	std::vector<Manifold> manifolds;
};
