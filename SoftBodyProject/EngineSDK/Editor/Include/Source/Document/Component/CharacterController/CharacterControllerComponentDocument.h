#pragma once

#include "FieldMacros.h"
#include "Base/ComponentDocument.h"

struct CharacterControllerComponentDocument :public ComponentDocument<CharacterControllerComponentDocument>
{
public:
	using ComponentDocument::ComponentDocument;

	void DrawGizmo(const GizmoContext& _context) override;
	bool AllowsMultiple() override;

public:
	bool motorEnabled{ true };

	float maxSpeed{ 500.0f };
	float maxMoverForceSlopeAngleDegrees{ 45.0f };
	float groundAcceleration{ 2000.0f };
	float groundDeceleration{ 2500.0f };

	float airAcceleration{ 500.0f };
	float airDeceleration{ 0.0f };
	float airForceScale{ 1.0f };
	float airSpeedScale{ 1.0f };
	float maxFallSpeed{ 1000.0f };

	float steepSlopeForceScale{ 1.0f };
	float steepSlopeSpeedScale{ 1.0f };

	float jumpSpeed{ 600.0f };
	float maxJumpTime{ 0.1f };

	float groundProbeDistance{ 10.0f };
	float minGroundDot{ 0.70710678f };
	float skirtRadius{ 20.0f };
	int skirtRayCount{ 8 };

	MD_DOCUMENT(
		CharacterControllerComponentDocument,
		"CharacterController",

		MD_FIELD("MotorEnabled", motorEnabled,
			MD_GUI_GROUP("Control"),
			MD_GUI_LABEL("Motor Enabled")),

		MD_FIELD("MaxSpeed", maxSpeed,
			MD_GUI_GROUP("Ground Movement"),
			MD_GUI_LABEL("Max Speed"),
			MD_GUI_MIN(0.0f),
			MD_GUI_SPEED(1.0f)),

		MD_FIELD("MaxMoverForceFromSlopeAngleDeg", maxMoverForceSlopeAngleDegrees,
			MD_GUI_GROUP("Ground Movement"),
			MD_GUI_LABEL("Hold Slope Angle"),
			MD_GUI_RANGE(0.0f, 90.0f),
			MD_GUI_SPEED(0.1f),
			MD_GUI_UNIT("deg"),
			MD_GUI_TOOLTIP("Maximum mover force is derived from this slope angle.")),

		MD_FIELD("GroundAcceleration", groundAcceleration,
			MD_GUI_GROUP("Ground Movement"),
			MD_GUI_LABEL("Acceleration"),
			MD_GUI_MIN(0.0f),
			MD_GUI_SPEED(10.0f)),

		MD_FIELD("GroundDeceleration", groundDeceleration,
			MD_GUI_GROUP("Ground Movement"),
			MD_GUI_LABEL("Deceleration"),
			MD_GUI_MIN(0.0f),
			MD_GUI_SPEED(10.0f)),

		MD_FIELD("AirAcceleration", airAcceleration,
			MD_GUI_GROUP("Air Movement"),
			MD_GUI_LABEL("Acceleration"),
			MD_GUI_MIN(0.0f),
			MD_GUI_SPEED(10.0f)),

		MD_FIELD("AirDeceleration", airDeceleration,
			MD_GUI_GROUP("Air Movement"),
			MD_GUI_LABEL("Deceleration"),
			MD_GUI_MIN(0.0f),
			MD_GUI_SPEED(10.0f)),

		MD_FIELD("AirForceScale", airForceScale,
			MD_GUI_GROUP("Air Movement"),
			MD_GUI_LABEL("Force Scale"),
			MD_GUI_MIN(0.0f),
			MD_GUI_SPEED(0.01f)),

		MD_FIELD("AirSpeedScale", airSpeedScale,
			MD_GUI_GROUP("Air Movement"),
			MD_GUI_LABEL("Speed Scale"),
			MD_GUI_MIN(0.0f),
			MD_GUI_SPEED(0.01f)),

		MD_FIELD("MaxFallSpeed", maxFallSpeed,
			MD_GUI_GROUP("Air Movement"),
			MD_GUI_LABEL("Max Fall Speed"),
			MD_GUI_MIN(0.0f),
			MD_GUI_SPEED(10.0f)),

		MD_FIELD("SteepSlopeForceScale", steepSlopeForceScale,
			MD_GUI_GROUP("Steep Slope"),
			MD_GUI_LABEL("Force Scale"),
			MD_GUI_MIN(0.0f),
			MD_GUI_SPEED(0.01f)),

		MD_FIELD("SteepSlopeSpeedScale", steepSlopeSpeedScale,
			MD_GUI_GROUP("Steep Slope"),
			MD_GUI_LABEL("Speed Scale"),
			MD_GUI_MIN(0.0f),
			MD_GUI_SPEED(0.01f)),

		MD_FIELD("JumpSpeed", jumpSpeed,
			MD_GUI_GROUP("Jump"),
			MD_GUI_LABEL("Jump Speed"),
			MD_GUI_MIN(0.0f),
			MD_GUI_SPEED(1.0f)),

		MD_FIELD("MaxJumpTime", maxJumpTime,
			MD_GUI_GROUP("Jump"),
			MD_GUI_LABEL("Max Jump Time"),
			MD_GUI_MIN(0.0f),
			MD_GUI_SPEED(0.01f),
			MD_GUI_UNIT("s")),

		MD_FIELD("GroundProbeDistance", groundProbeDistance,
			MD_GUI_GROUP("Ground Probe"),
			MD_GUI_LABEL("Support Distance"),
			MD_GUI_MIN(0.0f),
			MD_GUI_SPEED(0.1f),
			MD_GUI_TOOLTIP("Support distance and ground probe length in world units.")),

		MD_FIELD("MinGroundDot", minGroundDot,
			MD_GUI_GROUP("Ground Probe"),
			MD_GUI_LABEL("Min Ground Dot"),
			MD_GUI_RANGE(-1.0f, 1.0f),
			MD_GUI_SPEED(0.01f)),

		MD_FIELD("SkirtRadius", skirtRadius,
			MD_GUI_GROUP("Ground Probe"),
			MD_GUI_LABEL("Skirt Radius"),
			MD_GUI_MIN(0.0f),
			MD_GUI_SPEED(0.1f)),

		MD_FIELD("SkirtRayCount", skirtRayCount,
			MD_GUI_GROUP("Ground Probe"),
			MD_GUI_LABEL("Skirt Ray Count"),
			MD_GUI_MIN(1))
	);
};