#pragma once

#include <vector>

#include "MDMath.h"

#include "BufferBase.h"

class CollisionContactBuffer :public BufferBase
{
public:
	// 衝突したやつA
	std::vector<int> handleA;
	// 衝突したやつB
	std::vector<int> handleB;
	// 深度
	std::vector<float> depth;
	// 法線
	std::vector<Vector3> normal;
};
