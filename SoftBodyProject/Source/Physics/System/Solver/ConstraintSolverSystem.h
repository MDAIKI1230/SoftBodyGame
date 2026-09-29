#pragma once

#include "SolverBodyBuffer.h"
#include "ConstraintBuffer.h"

class ConstraintSolverSystem
{
public:
	// コンストラクタ
	ConstraintSolverSystem();
	// 拘束解消
	void Solve(SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer);
	// 位置解決
	void PositionSolver(SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer);
	// PBD法による位置解消関数
	void PBDPositionSolver(SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer);
	// 速度再計算
	void ReCalcVelocity(SolverBodyBuffer* _solverBodyBuffer);
private:
	// バネ定数
	const float K;
	// バネ定数×Δt
	const float K_DELTA_TIME;
	// 減衰定数
	const float C;
	// erp項
	const float ERP;
	// γ
	const float GAMMA;
};
