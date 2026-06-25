#pragma once

#include <vector>

#include "MDMath.h"

#include "BufferBase.h"

class CollisionContactBuffer :public BufferBase
{
public:
	// 深度
	std::vector<float> depth;
	// 法線
	std::vector<Vector3> normal;
};
