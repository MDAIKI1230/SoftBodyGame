#pragma once

#include <cstdint>
#include <type_traits>

enum class  RigidBodyUpdateFlag :uint16_t
{
	NONE = 0,
	POSITION = 1 << 0,
	VELOCITY = 1 << 1,
	FORCE = 1 << 2,
	ROTATION = 1 << 3,
	ANGULAR_VELOCITY = 1 << 4,
	TORQUE = 1 << 5,
	MASS = 1 << 6,
	IS_GRAVITY = 1 << 8,
	GRAVITY = 1 << 9
};

[[nodiscard]]
constexpr RigidBodyUpdateFlag operator|(RigidBodyUpdateFlag _lhs, RigidBodyUpdateFlag _rhs) noexcept
{
	using Type = std::underlying_type_t<RigidBodyUpdateFlag>;

	return static_cast<RigidBodyUpdateFlag>(static_cast<Type>(_lhs) | static_cast<Type>(_rhs));
}

constexpr RigidBodyUpdateFlag& operator|=(RigidBodyUpdateFlag& _lhs, RigidBodyUpdateFlag _rhs) noexcept
{
	_lhs = _lhs | _rhs;
	return _lhs;
}

[[nodiscard]]
constexpr bool HasFlag(RigidBodyUpdateFlag _value, RigidBodyUpdateFlag _flag) noexcept
{
	using Type = std::underlying_type_t<RigidBodyUpdateFlag>;

	return (static_cast<Type>(_value) & static_cast<Type>(_flag)) != 0;
}
