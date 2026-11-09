#pragma once

#include <cstdint>
#include <limits>
#include <string>
#include <vector>

#include "FieldMacros.h"
#include "MDMath.h"
#include "ColliderComponentData.h"

struct CapsuleColliderComponentData : public ColliderComponentData
{
public:
	std::string GetName() override { return "CapsuleCollider"; }
public:
	float height{ 1.0f };
	float radius{ 0.5f };
	Vector3 offsetPosition{};

	MD_DOCUMENT(
		CapsuleColliderComponentData, "CapsuleCollider",
		MD_FIELD("Height", height),
		MD_FIELD("Radius", radius),
		MD_FIELD("OffsetPosition", offsetPosition),
		MD_FIELD("Category", category)
	);
};