#pragma once

#include "CollisionConstants.h"

struct ColliderProjection
{
	// 射影値
	float projection;
	// エンティティ
	int entity;
	// 最大値フラグ
	bool isMax;
	// 値種類
	ProjectionAxisType axisType;
};
