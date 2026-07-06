#pragma once

#include "MDMath.h"

#include "SceneComponentData.h"

struct BoxColliderComponentData :public SceneComponentData
{
public:
	std::string GetName() override
	{
		return "BoxColliderComponent";
	}
public:
	Vector3 size;
};
