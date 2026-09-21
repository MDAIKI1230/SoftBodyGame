#pragma once

#include "MDMath.h"

#include "SceneComponentData.h"

struct BoxColliderComponentData :public SceneComponentData
{
public:
	std::string GetName() override
	{
		return "BoxCollider";
	}
public:
	Vector3 size;
	Vector3 offsetPosition;
	Vector3 offsetRotationDeg;
};
