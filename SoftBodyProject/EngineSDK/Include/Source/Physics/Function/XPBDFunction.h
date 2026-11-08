#pragma once

#include "SolverBody.h"
#include "SolverBodyBuffer.h"
#include "Constraint.h"

namespace XPBDFunction
{
	// XPBD法による位置解消関数
	void SolveRow(SolverBody& _solverBodyA, SolverBody& _solverBodyB, Constraint& _constraint);
	// 速度の再計算
	void ReCalcVelocity(SolverBodyBuffer* _solverBodyBuffer);
	// 目標速度に向かって力を加えるための関数
	void SolveTargetVelocity(SolverBody& _body, const Vector3& _targetVelocity, float _maxLambda);
}