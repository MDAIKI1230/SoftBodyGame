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

	void MakeConstraintInfo(Constraint& _constraint, const ConstraintTuning& _tuning);

	// 点拘束を情報からソルバ用拘束に変換してバッファに入れる(ソルバボディの情報だけ入れた奴を渡す)
	void AddPointConstraint(
		Constraint _constraint, const ConstraintTuning& _tuning,
		const Vector3& _rA, const Vector3& _pointA,
		const Vector3& _rB, const Vector3& _pointB,
		ConstraintBuffer* _constraintBuffer);

	// 軸合わせ用Angular Row追加(ソルバボディの情報だけ入れた奴を渡す)
	void AddAxisConstraint(
		Constraint _constraint, const ConstraintTuning& _tuning,
		const Vector3& _tangent, const Vector3& _axisError,
		ConstraintBuffer* _constraintBuffer);
};
