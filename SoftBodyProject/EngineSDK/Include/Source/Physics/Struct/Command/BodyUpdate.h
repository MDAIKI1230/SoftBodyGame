#pragma once

#include <cstdint>

#include "MDMath.h"

#include "BodyID.h"

#include "CommandConstants.h"

struct BodyUpdate
{
	BodyID target;

	Vector3 position;
	Vector3 velocity;
	Vector3 force;

	Quaternion rotation;
	Vector3 angularVelocity;
	Vector3 torque;

	float mass;

	bool isGravity;
	Vector3 gravity;

	RigidBodyUpdateFlag flags;
};
