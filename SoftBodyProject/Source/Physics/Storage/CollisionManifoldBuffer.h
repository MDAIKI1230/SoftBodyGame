#pragma once

#include <unordered_map>

#include "Manifold.h"
#include "CollisionPair.h"

class CollisionManifoldBuffer
{
public:
	std::unordered_map<CollisionPair::Pair, Manifold> manifolds;
};
