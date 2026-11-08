#pragma once

#include "FieldMacros.h"
#include "SceneComponentData.h"

struct CharacterControllerComponentData :public SceneComponentData
{
public:
	// コンストラクタ
	CharacterControllerComponentData()
	{
		creationPriority = 0;
	}

	std::string GetName() override { return "CharacterController"; }
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
		CharacterControllerComponentData, "CharacterController",
		MD_FIELD("MotorEnabled", motorEnabled),
		MD_FIELD("MaxSpeed", maxSpeed),
		MD_FIELD("MaxMoverForceFromSlopeAngleDeg", maxMoverForceSlopeAngleDegrees),
		MD_FIELD("GroundAcceleration", groundAcceleration),
		MD_FIELD("GroundDeceleration", groundDeceleration),
		MD_FIELD("AirAcceleration", airAcceleration),
		MD_FIELD("AirDeceleration", airDeceleration),
		MD_FIELD("AirForceScale", airForceScale),
		MD_FIELD("AirSpeedScale", airSpeedScale),
		MD_FIELD("MaxFallSpeed", maxFallSpeed),
		MD_FIELD("SteepSlopeForceScale", steepSlopeForceScale),
		MD_FIELD("SteepSlopeSpeedScale", steepSlopeSpeedScale),
		MD_FIELD("JumpSpeed", jumpSpeed),
		MD_FIELD("MaxJumpTime", maxJumpTime),
		MD_FIELD("GroundProbeDistance", groundProbeDistance),
		MD_FIELD("MinGroundDot", minGroundDot),
		MD_FIELD("SkirtRadius", skirtRadius),
		MD_FIELD("SkirtRayCount", skirtRayCount)
	);
};