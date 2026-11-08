#pragma once

#include <numbers>

namespace MathConstants
{
	inline constexpr float EPSILON{ 0.0001f };
	inline constexpr float PI_FLT{ std::numbers::pi_v<float> };
}
