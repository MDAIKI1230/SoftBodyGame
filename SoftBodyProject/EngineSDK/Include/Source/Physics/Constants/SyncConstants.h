#pragma once

#include <cstdint>

enum class PhysicsTransformSyncPolicy : uint8_t
{
    NONE = 0,
    READ_FROM_ECS = 1 << 0,
    WRITE_TO_ECS = 1 << 1,
    BIDIRECTIONAL = READ_FROM_ECS | WRITE_TO_ECS
};

[[nodiscard]]
constexpr bool HasFlag(PhysicsTransformSyncPolicy _value, PhysicsTransformSyncPolicy _flag) noexcept
{
    using Type = std::underlying_type_t<PhysicsTransformSyncPolicy>;

    return (static_cast<Type>(_value) & static_cast<Type>(_flag)) != 0;
}
