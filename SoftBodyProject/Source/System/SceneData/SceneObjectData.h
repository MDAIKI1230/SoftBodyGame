#pragma once

#include <string>
#include <vector>

#include "SceneComponentData.h"

struct SceneObjectData
{
	std::string name;
	std::vector<std::unique_ptr<SceneComponentData>> component;
};
