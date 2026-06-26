#pragma once

#include <vector>

class AABBBroadPhaseStorage
{
public:
	std::vector<float> minX;
	std::vector<float> minY;
	std::vector<float> minZ;

	std::vector<float> maxX;
	std::vector<float> maxY;
	std::vector<float> maxZ;
};
