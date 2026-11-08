#pragma once

#include <limits>

#include "FieldMacros.h"
#include "Base/ConstraintDocumentBase.h"

struct LimitedBallJointConstraintComponentDocument : public ConstraintDocumentBase<LimitedBallJointConstraintComponentDocument>
{
public:
	using ConstraintDocumentBase::ConstraintDocumentBase;

	void DrawGizmo(const GizmoContext& _context) override;

public:
	// エンジンへ適用するときにdegからradへ変換する。
	float swingAngleDegrees{ 0.0f };
	float twistAngleMinDegrees{ 0.0f };
	float twistAngleMaxDegrees{ 0.0f };

	// ConstraintTuningと同じ既定値。
	float positionCompliance{ 0.0f };
	float positionMaxForce{ (std::numeric_limits<float>::max)() };
	float angularCompliance{ 0.0f };
	float angularMaxForce{ (std::numeric_limits<float>::max)() };

	MD_DOCUMENT(
		LimitedBallJointConstraintComponentDocument,
		"LimitedBallJointConstraint",

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

		MD_FIELD("SwingAngleDeg", swingAngleDegrees,
			MD_GUI_GROUP("Limits"),
			MD_GUI_LABEL("Swing Angle"),
			MD_GUI_UNIT("deg"),
			MD_GUI_SPEED(0.1f)),

		MD_FIELD("TwistAngleMinDeg", twistAngleMinDegrees,
			MD_GUI_GROUP("Limits"),
			MD_GUI_LABEL("Twist Angle Min"),
			MD_GUI_UNIT("deg"),
			MD_GUI_SPEED(0.1f)),

		MD_FIELD("TwistAngleMaxDeg", twistAngleMaxDegrees,
			MD_GUI_GROUP("Limits"),
			MD_GUI_LABEL("Twist Angle Max"),
			MD_GUI_UNIT("deg"),
			MD_GUI_SPEED(0.1f)),

		MD_FIELD("PositionCompliance", positionCompliance,
			MD_GUI_GROUP("Position Tuning"),
			MD_GUI_LABEL("Compliance"),
			MD_GUI_MIN(0.0f),
			MD_GUI_SPEED(0.001f),
			MD_GUI_FORMAT("%.6g")),

		MD_FIELD("PositionMaxForce", positionMaxForce,
			MD_GUI_GROUP("Position Tuning"),
			MD_GUI_LABEL("Max Force"),
			MD_GUI_MIN(0.0f),
			MD_GUI_FORMAT("%.6g")),

		MD_FIELD("AngularCompliance", angularCompliance,
			MD_GUI_GROUP("Angular Tuning"),
			MD_GUI_LABEL("Compliance"),
			MD_GUI_MIN(0.0f),
			MD_GUI_SPEED(0.001f),
			MD_GUI_FORMAT("%.6g")),

		MD_FIELD("AngularMaxForce", angularMaxForce,
			MD_GUI_GROUP("Angular Tuning"),
			MD_GUI_LABEL("Max Force"),
			MD_GUI_MIN(0.0f),
			MD_GUI_FORMAT("%.6g"))
	);
};