#pragma once

#include <limits>

#include "FieldMacros.h"
#include "../Base/ComponentDocument.h"
#include "BodyConstants.h"

struct RigidBodyComponentDocument : public ComponentDocument<RigidBodyComponentDocument>
{
public:
	using ComponentDocument::ComponentDocument;

	RigidBodyPositionLock GetPositionLock() const;
	RigidBodyRotationLock GetRotationLock() const;

public:
	float mass{ 1.0f };
	bool isGravity{ true };
	Vector3 gravity{ 0.0f, -980.0f, 0.0f };
	Vector3 velocity{};
	// エンジンと同じく、角速度の単位はrad/s。
	Vector3 angularVelocity{};

	bool freezePositionX{ false };
	bool freezePositionY{ false };
	bool freezePositionZ{ false };
	bool freezeRotationX{ false };
	bool freezeRotationY{ false };
	bool freezeRotationZ{ false };

	MD_DOCUMENT(
		RigidBodyComponentDocument,
		"RigidBody",

		MD_FIELD("Mass", mass,
			MD_GUI_MIN((std::numeric_limits<float>::min)()),
			MD_GUI_SPEED(0.1f),
			MD_GUI_FORMAT("%.6g")),

		MD_FIELD("UseGravity", isGravity,
			MD_GUI_LABEL("Use Gravity")),

		MD_FIELD("Gravity", gravity,
			MD_GUI_ENABLED_BY(&RigidBodyComponentDocument::isGravity)),

		MD_FIELD("Velocity", velocity),

		MD_FIELD("AngularVelocity", angularVelocity,
			MD_GUI_LABEL("Angular Velocity"),
			MD_GUI_UNIT("rad/s"),
			MD_GUI_TOOLTIP("Angular velocity in radians per second.")),

		MD_FIELD("FreezePositionX", freezePositionX,
			MD_GUI_GROUP("Constraints"),
			MD_GUI_LABEL("Freeze Position"),
			MD_GUI_INPUT_LABEL("X")),

		MD_FIELD("FreezePositionY", freezePositionY,
			MD_GUI_GROUP("Constraints"),
			MD_GUI_SAME_LINE(),
			MD_GUI_INPUT_LABEL("Y")),

		MD_FIELD("FreezePositionZ", freezePositionZ,
			MD_GUI_GROUP("Constraints"),
			MD_GUI_SAME_LINE(),
			MD_GUI_INPUT_LABEL("Z")),

		MD_FIELD("FreezeRotationX", freezeRotationX,
			MD_GUI_GROUP("Constraints"),
			MD_GUI_LABEL("Freeze Rotation"),
			MD_GUI_INPUT_LABEL("X")),

		MD_FIELD("FreezeRotationY", freezeRotationY,
			MD_GUI_GROUP("Constraints"),
			MD_GUI_SAME_LINE(),
			MD_GUI_INPUT_LABEL("Y")),

		MD_FIELD("FreezeRotationZ", freezeRotationZ,
			MD_GUI_GROUP("Constraints"),
			MD_GUI_SAME_LINE(),
			MD_GUI_INPUT_LABEL("Z"))
	);
};