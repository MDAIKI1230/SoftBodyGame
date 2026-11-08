#pragma once

#include <limits>

#include "FieldMacros.h"
#include "Base/ConstraintDocumentBase.h"

struct AngleLimitPointConstraintComponentDocument : public ConstraintDocumentBase<AngleLimitPointConstraintComponentDocument>
{
public:
	using ConstraintDocumentBase::ConstraintDocumentBase;

	void DrawGizmo(const GizmoContext& _context) override;

public:
	// エンジンへ適用するときにdegからradへ変換する。
	float angleMinDegrees{ 0.0f };
	float angleMaxDegrees{ 0.0f };

	// ConstraintTuningと同じ既定値。
	float compliance{ 0.0f };
	float maxForce{ (std::numeric_limits<float>::max)() };

	MD_DOCUMENT(
		AngleLimitPointConstraintComponentDocument,
		"AngleLimitPointConstraint",

		MD_FIELD("LocalPosition", localPosition,
			MD_GUI_GROUP("Owner End Point"),
			MD_GUI_LABEL("Local Position")),

		MD_FIELD("LocalRotationDeg", localRotationDegrees,
			MD_GUI_GROUP("Owner End Point"),
			MD_GUI_LABEL("Local Rotation"),
			MD_GUI_UNIT("deg")),

		MD_FIELD("Weight", weight,
			MD_GUI_GROUP("Owner End Point"),
			MD_GUI_MIN(0.0f),
			MD_GUI_SPEED(0.01f)),

		MD_FIELD("EndPoints", endPoints,
			MD_GUI_GROUP("End Points"),
			MD_GUI_LABEL("End Points")),

		MD_FIELD("AngleMinDeg", angleMinDegrees,
			MD_GUI_GROUP("Limits"),
			MD_GUI_LABEL("Angle Min"),
			MD_GUI_UNIT("deg"),
			MD_GUI_SPEED(0.1f)),

		MD_FIELD("AngleMaxDeg", angleMaxDegrees,
			MD_GUI_GROUP("Limits"),
			MD_GUI_LABEL("Angle Max"),
			MD_GUI_UNIT("deg"),
			MD_GUI_SPEED(0.1f)),

		MD_FIELD("Compliance", compliance,
			MD_GUI_GROUP("Tuning"),
			MD_GUI_MIN(0.0f),
			MD_GUI_SPEED(0.001f),
			MD_GUI_FORMAT("%.6g")),

		MD_FIELD("MaxForce", maxForce,
			MD_GUI_GROUP("Tuning"),
			MD_GUI_LABEL("Max Force"),
			MD_GUI_MIN(0.0f),
			MD_GUI_FORMAT("%.6g"))
	);
};