#pragma once

#include <span>

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
	// 一軸方向だけ、距離拘束がかかる拘束
	void SolveAxisDistanceConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer, size_t& _batchCount);

	// --- 各種計算 ---

	// BodyとEndPointから位置などの計算をする
	void CalcEndPointPositionAndOffset(
		SolverBody& _body, const EndPointFrame& _endPoint,
		Vector3& _worldPosition, Vector3& _worldOffset);

	// 接続点の位置合わせ：3行
	void SolvePointWorldAxisRows(
		SolverBody& _bodyA, const EndPointFrame& _endPointA,
		SolverBody& _bodyB, const EndPointFrame& _endPointB,
		std::span<Constraint, 3> _rows);

	// EndPointAに追従する軸方向の位置合わせ：1行
	void SolvePointLocalAxisRow(
		SolverBody& _bodyA, const EndPointFrame& _endPointA,
		SolverBody& _bodyB, const EndPointFrame& _endPointB,
		Constraint& _row,
		const Vector3& _localAxis);

	// 距離固定：1行
	void SolveDistanceRow(
		SolverBody& _bodyA, const EndPointFrame& _endPointA,
		SolverBody& _bodyB, const EndPointFrame& _endPointB,
		Constraint& _row,
		float _distance);

	// ヒンジ軸合わせ：2行
	void SolveHingeAxisRows(
		SolverBody& _bodyA, const EndPointFrame& _endPointA,
		SolverBody& _bodyB, const EndPointFrame& _endPointB,
		std::span<Constraint, 2> _rows);

	// スウィング角度制限：1行
	void SolveSwingLimitRow(
		SolverBody& _bodyA, const EndPointFrame& _endPointA,
		SolverBody& _bodyB, const EndPointFrame& _endPointB,
		Constraint& _row,
		const Vector3& _localAxisA, const Vector3& _localAxisB,
		float _minAngle, float _maxAngle);

	// ツイスト角度制限：1行
	void SolveTwistLimitRow(
		SolverBody& _bodyA, const EndPointFrame& _endPointA,
		SolverBody& _bodyB, const EndPointFrame& _endPointB,
		Constraint& _row,
		float _minAngle, float _maxAngle);

	// 目標相対姿勢への姿勢合わせ：3行
	void SolveRotationRows(
		SolverBody& _bodyA, const EndPointFrame& _endPointA,
		SolverBody& _bodyB, const EndPointFrame& _endPointB,
		std::span<Constraint, 3> _rows,
		const Quaternion& _targetRelativeRotation);

	// 線上拘束：ローカルX・Z方向の2行
	void SolvePointOnLineRows(
		SolverBody& _bodyA, const EndPointFrame& _endPointA,
		SolverBody& _bodyB, const EndPointFrame& _endPointB,
		std::span<Constraint, 2> _rows);

	// EndPointAに追従する軸方向の移動制限：1行
	void SolveLinearLimitRow(
		SolverBody& _bodyA, const EndPointFrame& _endPointA,
		SolverBody& _bodyB, const EndPointFrame& _endPointB,
		Constraint& _row,
		const Vector3& _localAxis,
		float _minDistance, float _maxDistance);
};