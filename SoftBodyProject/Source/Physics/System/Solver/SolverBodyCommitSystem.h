#pragma once

#include "SolverBodyBuffer.h"

#include "PhysicsTransformStorage.h"
#include "RigidBodyStorage.h"

class SolverBodyCommitSystem
{
public:
	void Commit(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, SolverBodyBuffer* _solverBodyBuffer);
};
