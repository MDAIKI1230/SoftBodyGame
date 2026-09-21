#pragma once

#include <string>

#include "SceneComponentData.h"

struct RendererComponentData :public SceneComponentData
{
public:
	std::string GetName() override
	{
		return "Renderer";
	}
public:
	std::string modelPath;
};
