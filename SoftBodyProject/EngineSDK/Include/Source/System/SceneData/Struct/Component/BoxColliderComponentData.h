#pragma once

#include <cstdint>
#include <limits>
#include <string>
#include <vector>

#include "FieldMacros.h"
#include "MDMath.h"
#include "ColliderComponentData.h"

struct BoxColliderComponentData : public ColliderComponentData
{
public:
	std::string GetName() override { return "BoxCollider"; }
public:
	Vector3 size{ Vector3::ONE };
	Vector3 offsetPosition{};

	MD_DOCUMENT(
		BoxColliderComponentData, "BoxCollider",
		MD_FIELD("Size", size),
		MD_FIELD("OffsetPosition", offsetPosition),
		MD_FIELD("Category", category)
	);
};