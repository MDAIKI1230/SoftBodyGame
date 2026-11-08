#pragma once

#include "MDMath.h"

/*
	内部の計算用の構造体。
	SkeletonInstanceに直接書き込んで使う
*/
struct TwoBoneIK
{
	uint32_t rootBoneIndex{ 0 };
	uint32_t jointBoneIndex{ 0 };
	uint32_t endBoneIndex{ 0 };

	Vector3 rootPosition;
	Vector3 jointPosition;
	Vector3 endPosition;

	Vector3 targetPosition;
	Vector3 polePosition;

	float positionWeight{ 1.0f };
};