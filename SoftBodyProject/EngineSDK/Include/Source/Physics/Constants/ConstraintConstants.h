#pragma once

#include "Color.h"

enum class ConstraintType
{
	POINTS,
	DISTANCE,
	HINGE,
	ANGLE_LIMIT_POINT,
	ANGLE_LIMIT_HINGE,
	LIMITED_BALL_JOINT,
	JOINT_DRIVE,
	POINT_ON_LINE,
	SLIDER,
	LIMITED_POINT_ON_LINE,
	LIMITED_SLIDER,
	AXIS_DISTANCE
};

// 拘束のデバッグ表示で、EngineとEditorが共有する既定値。
struct ConstraintDebugRenderConstants
{
	static constexpr float AXIS_LENGTH{ 30.0f };
	static constexpr float POINT_RADIUS{ 3.0f };
	static constexpr float PROJECTED_POINT_RADIUS{ 2.0f };
	static constexpr float LIMIT_RING_RADIUS{ 5.0f };
	static constexpr int CURVE_DIVISIONS{ 32 };

	inline static const Color CONSTRAINT_COLOR{ 0.0f, 1.0f, 0.0f };
	inline static const Color ERROR_COLOR{ 1.0f, 0.0f, 0.0f };
};