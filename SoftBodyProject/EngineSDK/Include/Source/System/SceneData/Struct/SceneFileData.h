#pragma once

#include <unordered_map>

#include "SceneObjectData.h"

struct SceneFileData
{
	int version{ 0 };
	
	std::unordered_map<std::uint64_t, SceneObjectData> objectDatas;
};
