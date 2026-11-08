#pragma once

#include "FieldMacros.h"

#include "Base/ComponentDocument.h"

#include "MDMath.h"

struct CameraComponentDocument :public ComponentDocument<CameraComponentDocument>
{
public:
	using ComponentDocument::ComponentDocument;

	void DrawGizmo(const GizmoContext& _context) override;
	bool AllowsMultiple() override;
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
		CameraComponentDocument,
		"Camera",

		MD_FIELD("IsMain", isMain,
			MD_GUI_LABEL("Main Camera")),

		MD_FIELD("IsActive", isActive,
			MD_GUI_LABEL("Active")),

		MD_FIELD("Priority", priority),

		MD_FIELD("Near", nearClip,
			MD_GUI_GROUP("Lens"),
			MD_GUI_LABEL("Near Clip"),
			MD_GUI_MIN(0.001f),
			MD_GUI_CLAMP(),
			MD_GUI_SPEED(0.1f)),

		MD_FIELD("Far", farClip,
			MD_GUI_GROUP("Lens"),
			MD_GUI_LABEL("Far Clip"),
			MD_GUI_MIN(0.001f),
			MD_GUI_CLAMP(),
			MD_GUI_SPEED(1.0f)),

		MD_FIELD("Fov", fov,
			MD_GUI_GROUP("Lens"),
			MD_GUI_LABEL("Field of View"),
			MD_GUI_RANGE(1.0f, 179.0f),
			MD_GUI_CLAMP(),
			MD_GUI_UNIT("deg")),

		MD_FIELD("ViewPortX", viewPortX,
			MD_GUI_GROUP("Viewport"),
			MD_GUI_LABEL("Position X"),
			MD_GUI_RANGE(0.0f, 1.0f),
			MD_GUI_CLAMP(),
			MD_GUI_SPEED(0.01f)),

		MD_FIELD("ViewPortY", viewPortY,
			MD_GUI_GROUP("Viewport"),
			MD_GUI_LABEL("Position Y"),
			MD_GUI_RANGE(0.0f, 1.0f),
			MD_GUI_CLAMP(),
			MD_GUI_SPEED(0.01f)),

		MD_FIELD("ViewPortWidth", viewPortWidth,
			MD_GUI_GROUP("Viewport"),
			MD_GUI_LABEL("Width"),
			MD_GUI_RANGE(0.001f, 1.0f),
			MD_GUI_CLAMP(),
			MD_GUI_SPEED(0.01f)),

		MD_FIELD("ViewPortHeight", viewPortHeight,
			MD_GUI_GROUP("Viewport"),
			MD_GUI_LABEL("Height"),
			MD_GUI_RANGE(0.001f, 1.0f),
			MD_GUI_CLAMP(),
			MD_GUI_SPEED(0.01f))
	);
private:
	static Matrix4x4 GetCameraRotation(const GizmoContext& _context);
};