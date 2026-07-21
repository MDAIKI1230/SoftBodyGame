#pragma once

#include "MDMath.h"

#include "SceneComponentData.h"

struct TransformComponentData :public SceneComponentData
{
public:
	std::string GetName() override
	{
		return "Transform";
	}
public:
	Vector3 position;
	Vector3 ratationDeg;
	Vector3 scale;
};
