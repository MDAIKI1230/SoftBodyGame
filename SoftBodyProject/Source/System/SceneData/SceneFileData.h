#pragma once

#include <vector>

#include "SceneObjectData.h"

struct SceneFileData
{
	int version{ 0 };
	
	std::vector<SceneObjectData> objectDatas;
};
