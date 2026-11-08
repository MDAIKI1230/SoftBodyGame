#pragma once

#include "FieldMacros.h"

#include "../Base/ComponentDocument.h"

#include "MDMath.h"

struct CapsuleColliderComponentDocument : public ComponentDocument<CapsuleColliderComponentDocument>
{
public:
	using ComponentDocument::ComponentDocument;

	void DrawGizmo(const GizmoContext& _context) override;

public:
	// 上下の半球の中心間の距離。全高はheight + radius * 2.0f。
	float height{ 1.0f };
	float radius{ 0.5f };
	// ローカルY軸を中心軸とし、Transformに対する位置を指定する。
	Vector3 offsetPosition{};

	MD_DOCUMENT(
		CapsuleColliderComponentDocument,
		"CapsuleCollider",

		MD_FIELD("Height", height,
			MD_GUI_MIN(0.0f),
			MD_GUI_SPEED(0.01f),
			MD_GUI_TOOLTIP("Distance between hemisphere centers. Total height is Height + 2 * Radius.")),

		MD_FIELD("Radius", radius,
			MD_GUI_MIN(0.0f),
			MD_GUI_SPEED(0.01f)),

		MD_FIELD("OffsetPosition", offsetPosition,
			MD_GUI_LABEL("Offset Position"))
	);
};