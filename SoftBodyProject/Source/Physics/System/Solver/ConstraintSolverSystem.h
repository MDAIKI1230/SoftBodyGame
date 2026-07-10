#pragma once

#include "SolverBodyBuffer.h"
#include "ConstraintBuffer.h"

class ConstraintSolverSystem
{
public:
	// コンストラクタ
	ConstraintSolverSystem();
	// 更新
	void FixedUpdate(SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer);
	// 拘束解消
	void ConstraintSolver(SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer);
	// 再計算
	void ReCalcPosRot(SolverBodyBuffer* _solverBodyBuffer);
	// 位置解決
	void PositionSolver(SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer);
private:
	// 速度解消回数
	static constexpr float VELOCITY_SOLVER_TIMES{ 10 };
	// 位置/姿勢解消回数
	static constexpr float POS_ROT_SOLVER_TIMES{ 4 };
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
