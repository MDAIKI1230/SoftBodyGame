#pragma once

#include <vector>

#include "EndPoint.h"
#include "ConstraintTuning.h"

struct AngleLimitPointConstraintComponent
{
	// 拘束のメンバー
	std::vector<EndPoint> endPoints;
	// 距離
	float distance{ 0.0f };

	// 柔らかさなどの調整用数値
	ConstraintTuning tuning;
};