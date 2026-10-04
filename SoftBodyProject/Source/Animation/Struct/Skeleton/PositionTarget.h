#pragma once

/*
	特定のボーンに位置を指定する際に使われる構造体
	ボーンインデックスとワールド位置を持つ
*/

#include <cstdint>

#include "MDMath.h"

struct PositionTarget
{
	uint32_t boneIndex;
	Vector3 worldPosition;
};