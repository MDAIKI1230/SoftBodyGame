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
	Vector3 diffuseColor{ 1.0f, 1.0f, 1.0f };
	Vector3 ambientColor{ 0.0f, 0.0f, 0.0f };
	bool enabled{ true };
	float intensity{ 1.0f };
	float range{ 0.0f };
	float innerAngleDegrees{ 0.0f };
	float outerAngleDegrees{ 0.0f };

	MD_DOCUMENT(
		LightComponentData, "Light",
		MD_FIELD("Type", type),
		MD_FIELD("DiffuseColor", diffuseColor),
		MD_FIELD("AmbientColor", ambientColor),
		MD_FIELD("Enabled", enabled),
		MD_FIELD("Intensity", intensity),
		MD_FIELD("Range", range),
		MD_FIELD("InnerAngleDeg", innerAngleDegrees),
		MD_FIELD("OuterAngleDeg", outerAngleDegrees)
	);
};