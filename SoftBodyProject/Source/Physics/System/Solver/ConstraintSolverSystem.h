#pragma once

#include "SolverBodyBuffer.h"
#include "ConstraintBuffer.h"
#include "ConstraintStorage.h"

class ConstraintSolverSystem
{
public:
	// コンストラクタ
	ConstraintSolverSystem() = default;
	void Solve(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer);
	// 速度再計算
	void ReCalcVelocity(SolverBodyBuffer* _solverBodyBuffer);

private:
	// 拘束生成
	void Build(
		ConstraintBuffer* _constraintBuffer, const ConstraintTuning& _tuning,
		float _weightA, float _weightB, size_t _buildSize, size_t _batchCount);
	// 拘束生成
	void Build(
		ConstraintBuffer* _constraintBuffer,
		const ConstraintTuning& _positionTuning, size_t _positionSize,
		const ConstraintTuning& _angulerTuning, size_t _angulerSize,
		float _weightA, float _weightB,
		size_t _batchCount);
	// 情報の作成関数
	void MakeConstraintInfo(Constraint& _constraint, const ConstraintTuning& _tuning);

	// XPBD法による位置解消関数
	void SolveRow(SolverBody& _solverBodyA, SolverBody& _solverBodyB, Constraint& _constraint);

	// 点拘束の解く用の拘束のヤコビアンと違反値の再計算をしながら解く
	void SolvePointConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer, size_t& _batchCount);
	// 距離拘束の解く用の拘束のヤコビアンと違反値の再計算をしながら解く
	void SolveDistanceConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer, size_t& _batchCount);
	// ヒンジ拘束の解く用の拘束のヤコビアンと違反値のをしながら解く
	void SolveHingeConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer, size_t& _batchCount);
	// 角度制限付き点拘束の解く用の拘束のヤコビアンと違反値の再計算をしながら解く
	void SolveAngleLimitPointConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer, size_t& _batchCount);
	// 角度制限付きヒンジ拘束の解く用の拘束のヤコビアンと違反値の再計算をしながら解く
	void SolveAngleLimitHingeConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer, size_t& _batchCount);
	// SwingTwist拘束の解く用の拘束のヤコビアンと違反値の再計算をしながら解く
	void SolveLimitedBallJointConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer, size_t& _batchCount);
	// 関節駆動拘束の解く用の拘束のヤコビアンと違反値の再計算をしながら解く
	void SolveJointDriveConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer, size_t& _batchCount);
	// ある点を線上の動きに制限する拘束のヤコビアンと違反値の計算
	void SolvePointOnLineConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer, size_t& _batchCount);
	// スライダー拘束のヤコビアンと違反値の計算
	void SolveSliderConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer, size_t& _batchCount);
	// 距離制限のある、ある点を線上の動きに制限する拘束のヤコビアンと違反値の計算
	void SolveLimitedPointOnLineConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer, size_t& _batchCount);
	// 距離制限のある、スライダー拘束のヤコビアンと違反値の計算
	void SolveLimitedSliderConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer, size_t& _batchCount);
};
