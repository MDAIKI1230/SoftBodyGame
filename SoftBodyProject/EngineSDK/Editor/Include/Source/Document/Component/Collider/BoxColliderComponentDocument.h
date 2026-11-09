#pragma once

#include "FieldMacros.h"

#include "../Base/ComponentDocument.h"

#include "MDMath.h"

struct BoxColliderComponentDocument : public ComponentDocument<BoxColliderComponentDocument>
{
public:
	using ComponentDocument::ComponentDocument;

	void DrawGizmo(const GizmoContext& _context) override;

public:
	// 各軸方向の全長。
	Vector3 size{ Vector3::ONE };
	// Transformに対するローカル位置。
	Vector3 offsetPosition{};
	int category{ -1 };

	MD_DOCUMENT(
		BoxColliderComponentDocument,
		"BoxCollider",

		MD_FIELD("Size", size,
			MD_GUI_MIN(0.0f),
			MD_GUI_SPEED(0.01f)),

		MD_FIELD("OffsetPosition", offsetPosition,
			MD_GUI_LABEL("Offset Position")),

		MD_FIELD("Category", category, InspectorFieldDrawFunc::CollisionCategory{})
	);
};