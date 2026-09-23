#pragma once

#include <string>

#include "ActiveRagdollDefinition.h"

class ActiveRagdollLoader
{
public:
	static bool Load(const std::string& _path, ActiveRagdollDefinition& _output);
};