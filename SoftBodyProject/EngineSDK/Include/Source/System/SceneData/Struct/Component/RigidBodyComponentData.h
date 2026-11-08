#pragma once

#include <cstdint>
#include <limits>
#include <string>
#include <vector>

#include "FieldMacros.h"
#include "MDMath.h"
#include "SceneComponentData.h"

struct RigidBodyComponentData : public SceneComponentData
{
public:
	std::string GetName() override { return "RigidBody"; }
public:
	float mass{ 1.0f };
	bool isGravity{ true };
	Vector3 gravity{ 0.0f, -980.0f, 0.0f };
	Vector3 velocity{};
	Vector3 angularVelocity{};
	bool freezePositionX{ false };
	bool freezePositionY{ false };
	bool freezePositionZ{ false };
	bool freezeRotationX{ false };
	bool freezeRotationY{ false };
	bool freezeRotationZ{ false };

	MD_DOCUMENT(
		RigidBodyComponentData, "RigidBody",
		MD_FIELD("Mass", mass),
		MD_FIELD("UseGravity", isGravity),
		MD_FIELD("Gravity", gravity),
		MD_FIELD("Velocity", velocity),
		MD_FIELD("AngularVelocity", angularVelocity),
		MD_FIELD("FreezePositionX", freezePositionX),
		MD_FIELD("FreezePositionY", freezePositionY),
		MD_FIELD("FreezePositionZ", freezePositionZ),
		MD_FIELD("FreezeRotationX", freezeRotationX),
		MD_FIELD("FreezeRotationY", freezeRotationY),
		MD_FIELD("FreezeRotationZ", freezeRotationZ)
	);
};