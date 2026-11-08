#pragma once

namespace Config
{
	inline constexpr int WINDOW_SIZE_W{ 1000 };
	inline constexpr int WINDOW_SIZE_H{ 500 };
	inline constexpr int COLOR_BIT{ 32 };
	inline constexpr float FPS{ static_cast<float>(60) };
	inline constexpr char INITIAL_SCENE[]{ "DebugScene" };
	inline constexpr bool WINDOW_MODE{ true };
}

enum Axis
{
	X,
	Y,
	Z
};