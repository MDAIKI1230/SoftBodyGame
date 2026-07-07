#pragma once

#include <string>
#include <memory>
#include <vector>

#include "SceneComponentData.h"

struct SceneObjectData
{
	std::string id;
	std::string type;
	std::vector<std::unique_ptr<SceneComponentData>> component;
};
