#pragma once

#include "ConstraintStorage.h"

#include "SolverBodyBuffer.h"
#include "ConstraintBuffer.h"

class ConstraintBuildSystem
{
public:
	void FixedUpdate(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer);
private:
	void BuildPointConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer);
	void BuildDistanceConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer);
};
