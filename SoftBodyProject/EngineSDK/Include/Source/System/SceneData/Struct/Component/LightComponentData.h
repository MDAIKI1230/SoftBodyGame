#pragma once

#include "FieldMacros.h"
#include "LightConstants.h"
#include "MDMath.h"
#include "SceneComponentData.h"

struct LightComponentData : public SceneComponentData
{
public:
	std::string GetName() override { return "Light"; }

public:
	LightType type{ LightType::DIRECTIONAL };
	Vector3 color{ 1.0f, 1.0f, 1.0f };
	bool enabled{ true };
	float intensity{ 1.0f };
	float range{ 0.0f };
	float innerAngleDegrees{ 0.0f };
	float outerAngleDegrees{ 0.0f };

	MD_DOCUMENT(
		LightComponentData, "Light",
		MD_FIELD("Type", type),
		MD_FIELD("Color", color),
		MD_FIELD("Enabled", enabled),
		MD_FIELD("Intensity", intensity),
		MD_FIELD("Range", range),
		MD_FIELD("InnerAngleDeg", innerAngleDegrees),
		MD_FIELD("OuterAngleDeg", outerAngleDegrees)
	);
};