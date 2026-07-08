#pragma once

#include "SolverBodyBuffer.h"
#include "ConstraintBuffer.h"

class ConstraintSolverSystem
{
public:
	// 拘束解消
	void ConstraintSolver(SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer);
};
