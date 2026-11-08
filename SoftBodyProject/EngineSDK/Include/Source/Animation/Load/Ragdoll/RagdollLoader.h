#pragma once

#include <string>

#include "RagdollDefinition.h"

class RagdollLoader
{
public:
	static bool Load(const std::string& _path, RagdollDefinition& _output);
};