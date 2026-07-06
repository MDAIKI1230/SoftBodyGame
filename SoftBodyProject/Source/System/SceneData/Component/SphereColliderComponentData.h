#pragma once

#include "SceneComponentData.h"

struct SphereColliderComponentData :public SceneComponentData
{
public:
	std::string GetName() override
	{
		return "SphereColliderComponent";
	}
public:
	float radius;
};
