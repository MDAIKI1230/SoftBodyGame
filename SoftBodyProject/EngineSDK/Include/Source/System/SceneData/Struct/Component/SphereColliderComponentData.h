#pragma once

#include <cstdint>
#include <limits>
#include <string>
#include <vector>

#include "FieldMacros.h"
#include "MDMath.h"
#include "SceneComponentData.h"

struct SphereColliderComponentData : public SceneComponentData
{
public:
	std::string GetName() override { return "SphereCollider"; }
public:
	float radius{ 1.0f };
	Vector3 offsetPosition{};

	MD_DOCUMENT(
		SphereColliderComponentData, "SphereCollider",
		MD_FIELD("Radius", radius),
		MD_FIELD("OffsetPosition", offsetPosition)
	);
};