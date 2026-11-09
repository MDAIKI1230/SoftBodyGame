#pragma once

#include "FieldMacros.h"

#include "../Base/ComponentDocument.h"

#include "MDMath.h"

struct SphereColliderComponentDocument : public ComponentDocument<SphereColliderComponentDocument>
{
public:
	using ComponentDocument::ComponentDocument;

	void DrawGizmo(const GizmoContext& _context) override;

public:
	float radius{ 1.0f };
	// Transformに対するローカル位置。
	Vector3 offsetPosition{};
	int category{ -1 };

	MD_DOCUMENT(
		SphereColliderComponentDocument,
		"SphereCollider",

		MD_FIELD("Radius", radius,
			MD_GUI_MIN(0.0f),
			MD_GUI_SPEED(0.01f)),

		MD_FIELD("OffsetPosition", offsetPosition,
			MD_GUI_LABEL("Offset Position")),

		MD_FIELD("Category", category, InspectorFieldDrawFunc::CollisionCategory{})
	);
};