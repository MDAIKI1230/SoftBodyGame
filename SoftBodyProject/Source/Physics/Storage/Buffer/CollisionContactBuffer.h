#pragma once

#include <vector>

#include "MDMath.h"

class CollisionContactBuffer
{
public:
	// 深度
	std::vector<float> depth;
	// 法線
	std::vector<Vector3> normal;
};
