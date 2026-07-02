#pragma once

#include <vector>

#include "Manifold.h"

class CollisionManifoldBuffer
{
public:
	void Clear()
	{
		for (auto& manifold : manifolds)
		{
			manifold.Clear();
		}
	}
public:
	std::vector<Manifold> manifolds;
};
