#pragma once

#include "SolverBodyBuffer.h"

#include "PhysicsTransformStorage.h"
#include "BodyStorage.h"

class SolverBodyCommitSystem
{
public:
	void Commit(PhysicsTransformStorage* _transformStorage, BodyStorage* _bodyStorage, SolverBodyBuffer* _solverBodyBuffer);
};
