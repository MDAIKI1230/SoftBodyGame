#pragma once

#include "FieldMacros.h"
#include "SceneComponentData.h"

struct CameraComponentData :public SceneComponentData
{
public:
	std::string GetName() override { return "Camera"; }
public:
	bool isMain{ false };
	bool isActive{ true };
	int priority{ 0 };

	float nearClip{ 1.0f };
	float farClip{ 2000.0f };
	float fov{ 60.0f };

	float viewPortX{ 0.0f };
	float viewPortY{ 0.0f };
	float viewPortWidth{ 1.0f };
	float viewPortHeight{ 1.0f };

	MD_DOCUMENT(
		CameraComponentData, "Camera",
		MD_FIELD("IsMain", isMain),
		MD_FIELD("IsActive", isActive),
		MD_FIELD("Priority", priority),
		MD_FIELD("Near", nearClip),
		MD_FIELD("Far", farClip),
		MD_FIELD("Fov", fov),
		MD_FIELD("ViewPortX", viewPortX),
		MD_FIELD("ViewPortY", viewPortY),
		MD_FIELD("ViewPortWidth", viewPortWidth),
		MD_FIELD("ViewPortHeight", viewPortHeight)
	);
};