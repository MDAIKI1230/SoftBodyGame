#pragma once

#include <vector>

#include "Manifold.h"

class CollisionManifoldBuffer
{
public:
	std::vector<Manifold> manifolds;
};
