#pragma once

#include <vector>
#include <unordered_map>

#include "SolverBody.h"

class SolverBodyBuffer
{
public:
	void AddSolverBody(PhysicsTransformID _transformID, const SolverBody& _body)
	{
		bodyMap[_transformID] = solverBodies.size();
		solverBodies.push_back(_body);
	}
	void Clear()
	{
		solverBodies.clear();
		bodyMap.clear();
	}
public:
	std::vector<SolverBody> solverBodies;
	std::unordered_map<PhysicsTransformID, uint32_t> bodyMap;
};
