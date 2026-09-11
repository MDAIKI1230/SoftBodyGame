#pragma once

#include "ConstraintStorage.h"

#include "SolverBodyBuffer.h"
#include "ConstraintBuffer.h"

class ConstraintBuildSystem
{
public:
	void FixedUpdate(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer);
private:
	// 点拘束の解く用の拘束構造体を作る
	void BuildPointConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer);
	// 距離拘束の解く用の拘束構造体を作る
	void BuildDistanceConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer);
	// ヒンジ拘束の解く用の拘束構造体を作る
	void BuildHingeConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer);
	// 角度制限付き点拘束の解く用の拘束構造体を作る
	void BuildAngleLimitPointConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer);
	// 角度制限付きヒンジ拘束の解く用の拘束構造体を作る
	void BuildAngleLimitHingeConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer);

	template<class T>
	void MakeConstraintInfo(Constraint& _constraint, const T& _base);
};
