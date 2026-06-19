#pragma once

class AABBBroadPhaseStorage
{
public:
	float* minX;
	float* minY;
	float* minZ;

	float* maxX;
	float* maxY;
	float* maxZ;
};
