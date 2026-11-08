#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

#include "SceneComponentData.h"
#include "TransformComponentData.h"

struct SceneObjectData
{
	std::uint64_t id{ 0 };
	std::string type;
	TransformComponentData transform;
	std::unordered_map<std::string, std::vector<std::unique_ptr<SceneComponentData>>> components;
	// componentsが所有するDataを、生成優先度順に参照する。
	std::vector<SceneComponentData*> componentCreationOrder;
	std::uint64_t parent{ 0 };
};