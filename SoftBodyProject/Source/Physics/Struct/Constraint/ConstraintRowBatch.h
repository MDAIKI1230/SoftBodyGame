#pragma once

#include <cstdint>
#include "ConstraintID.h"

/*
	拘束が、解く用拘束のどの範囲を持っているかを記録する用構造体
	違反値などの再計算に用いる。
*/
struct ConstraintRowBatch
{
	ConstraintID sourceConstraintID;
	size_t endpointIndex{ 0 };

	size_t firstRow{ 0 };
	size_t rowCount{ 0 };
};