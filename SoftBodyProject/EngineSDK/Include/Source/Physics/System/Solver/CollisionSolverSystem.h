#pragma once

#include <vector>

#include "CollisionManifoldBuffer.h"
#include "SolverBodyBuffer.h"

#include "ColliderStorage.h"

#include "ContactConstraint.h"
#include "Constraint.h"
#include "SolverBody.h"

class CollisionSolverSystem
{
public:
	// コンストラクタ
	CollisionSolverSystem() = default;
	// 準備
	void StartUp(ColliderStorage* _colliderStorage, CollisionManifoldBuffer* _manifoldBuffer, SolverBodyBuffer* _solverBodyBuffer);

	void Solve(SolverBodyBuffer* _solverBodyBuffer);
	void VelocitySolve(SolverBodyBuffer* _solverBodyBuffer);
	

	// 終わり
	void End(CollisionManifoldBuffer* _manifoldBuffer);
private:
	void CalcContactRow(SolverBody& _bodyA, SolverBody& _bodyB, const ContactConstraint& _contact, Constraint& _row);
	void FrictionSolve(SolverBodyBuffer* _solverBodyBuffer);
private:
	std::vector<ContactConstraint> contactConstraints;
	std::vector<Constraint> constraints;
};
