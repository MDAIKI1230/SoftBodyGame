#pragma once

#include "BoneMask.h"
#include "SkeletonData.h"

class BoneMaskLoader
{
public:
	static bool Load(const char* _path, const SkeletonData* _skeleton, BoneMask& _output);
};