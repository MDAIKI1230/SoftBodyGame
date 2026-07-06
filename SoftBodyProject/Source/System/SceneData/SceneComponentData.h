#pragma once

#include <string>

struct SceneComponentData
{
public:
	virtual std::string GetName() = 0;
	virtual ~SceneComponentData() = default;
};
