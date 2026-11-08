#pragma once

#include <string>

#include "SkeletonData.h"
#include "ActiveRagdollDefinition.h"

class ActiveRagdollLoader
{
public:
	static bool Load(const std::string& _path, const SkeletonData* _skeleton, ActiveRagdollDefinition& _output);
};