#pragma once

#include "CollisionConstants.h"

struct ColliderProjection
{
	// 射影値
	float projection;
	// コライダーID
	ColliderID colliderID;
	// 最大値フラグ
	bool isMax;
	// 値種類
	ProjectionAxisType axisType;
	// アクティブIndex
	uint32_t activeIndex;
};
