#pragma once

#include <cstdint>
#include <type_traits>

enum class BodyType
{
	RIGID_BODY,
	ROPE,
	CLOTH,
	SOFT_BODY
};

enum class RigidBodyRotationLock :uint8_t
{
	NONE = 0,

	X = 1 << 0,
	Y = 1 << 1,
	Z = 1 << 2,

	ALL = (1 << 0) | (1 << 1) | (1 << 2)
};

constexpr RigidBodyRotationLock operator|(RigidBodyRotationLock _lhs, RigidBodyRotationLock _rhs)
{
	using Type = std::underlying_type_t<RigidBodyRotationLock>;

	return static_cast<RigidBodyRotationLock>(
		static_cast<Type>(_lhs) |
		static_cast<Type>(_rhs));
}

enum class RigidBodyPositionLock : std::uint8_t
{
	NONE = 0,

	X = 1 << 0,
	Y = 1 << 1,
	Z = 1 << 2,

	ALL = (1 << 0) | (1 << 1) | (1 << 2)
};

constexpr RigidBodyPositionLock operator|(RigidBodyPositionLock _lhs, RigidBodyPositionLock _rhs)
{
	using Type = std::underlying_type_t<RigidBodyPositionLock>;

	return static_cast<RigidBodyPositionLock>(
		static_cast<Type>(_lhs) |
		static_cast<Type>(_rhs));
}
