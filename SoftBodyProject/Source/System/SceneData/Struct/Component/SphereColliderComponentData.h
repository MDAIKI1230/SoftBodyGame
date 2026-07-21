#pragma once

#include "SceneComponentData.h"

struct SphereColliderComponentData :public SceneComponentData
{
public:
	std::string GetName() override
	{
		return "SphereCollider";
	}
public:
	float radius{ 0.0f };
};
