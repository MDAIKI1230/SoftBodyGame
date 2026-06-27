#pragma once

#include <vector>

#include "ColliderID.h"

#include "ColliderProjectionData.h"

class ProjectionStorage
{
	// コライダーIDのindexに対応させる。
	std::vector<ColliderProjectionData> data;
};
