#pragma once

#include <cstdint>

#include "MDMath.h"

struct BodyUpdate
{
	Vector3 position;
	Vector3 velocity;
	Vector3 force;

	Quaternion rotation;
	Vector3 angularVelocity;
	Vector3 torque;

	float mass;

	Matrix4x4 inertiaTensor;

	bool isGravity;
	Vector3 gravity;

	uint16_t diary;
};
