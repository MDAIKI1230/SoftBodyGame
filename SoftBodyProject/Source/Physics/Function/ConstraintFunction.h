#pragma once

#include <span>

#include "Constraint.h"

namespace ConstraintFunction
{
	// 点拘束のヤコビアンと違反値の計算
	void CalcPointJacobianAndError(
		const Vector3& _positionA, const Vector3& _rA,
		const Vector3& _positionB, const Vector3& _rB,
		std::span<Constraint, 3> _output);
	// 距離拘束のヤコビアンと違反値の計算
	void CalcDistanceJacobianAndError(
		const float _distance,
		const Vector3& _positionA, const Vector3& _rA,
		const Vector3& _positionB, const Vector3& _rB,
		Constraint& _output);
	// ヒンジ拘束のヤコビアンと違反値の計算
	void CalcHingeJacobianAndError(
		const Vector3& _positionA, const Vector3& _rA, const Vector3& _axisA,
		const Vector3& _positionB, const Vector3& _rB, const Vector3& _axisB,
		std::span<Constraint, 5> _output);
	// 角度制限付き点拘束のヤコビアンと違反値の計算
	void CalcAngleLimitPointJacobianAndError(
		const float _angleMax, const float _angleMin,
		const Vector3& _positionA, const Vector3& _rA, const Vector3& _dirA,
		const Vector3& _positionB, const Vector3& _rB, const Vector3& _dirB,
		std::span<Constraint, 4> _output);
	// 角度制限付きヒンジ拘束のヤコビアンと違反値の計算
	void CalcAngleLimitHingeJacobianAndError(
		const float _angleMax, const float _angleMin,
		const Vector3& _positionA, const Vector3& _rA, const Vector3& _axisA, const Vector3 _referenceA,
		const Vector3& _positionB, const Vector3& _rB, const Vector3& _axisB, const Vector3 _referenceB,
		std::span<Constraint, 6> _output);
	// SwingTwist拘束のヤコビアンと違反値の計算
	void CalcLimitedBallJointJacobianAndError(
		const float _swingAngle, const float _twistAngleMax, const float _twistAngleMin,
		const Vector3& _positionA, const Vector3& _rA, const Vector3& _axisA, const Vector3 _referenceA,
		const Vector3& _positionB, const Vector3& _rB, const Vector3& _axisB, const Vector3 _referenceB,
		std::span<Constraint, 5> _output);
	// 関節駆動拘束のヤコビアンと違反値の計算
	void CalcJointDriveJacobianAndError(
		const Quaternion& _targetRelativeRotation,
		const Quaternion& _rotationA,
		const Quaternion& _rotationB,
		std::span<Constraint, 3> _output);
	// ある点を線上の動きに制限する拘束のヤコビアンと違反値の計算
	void CalcPointOnLineJacobianAndError(
		const Quaternion& _rotationA, const Vector3& _positionA, const Vector3& _rA,
		const Vector3& _positionB, const Vector3& _rB,
		std::span<Constraint, 2> _output);
	// スライダー拘束のヤコビアンと違反値の計算
	void CalcSliderJacobianAndError(
		const Quaternion& _rotationA, const Vector3& _positionA, const Vector3& _rA,
		const Quaternion& _rotationB, const Vector3& _positionB, const Vector3& _rB,
		std::span<Constraint, 5> _output);
	// 距離制限のある、ある点を線上の動きに制限する拘束のヤコビアンと違反値の計算
	void CalcLimitedPointOnLineJacobianAndError(
		const float _distance,
		const Quaternion& _rotationA, const Vector3& _positionA, const Vector3& _rA,
		const Vector3& _positionB, const Vector3& _rB,
		std::span<Constraint, 3> _output);
	// 距離制限のある、スライダー拘束のヤコビアンと違反値の計算
	void CalcLimitedSliderJacobianAndError(
		const float _distance,
		const Quaternion& _rotationA, const Vector3& _positionA, const Vector3& _rA,
		const Quaternion& _rotationB, const Vector3& _positionB, const Vector3& _rB,
		std::span<Constraint, 6> _output);

	// 軸合わせヤコビアンと違反値の計算
	void CalcAxisJacobianAndError(const Vector3& _tangent, const Vector3& _axisError, Constraint& _output);
	// 軸に対して垂直なベクトル2つを生成する関数
	void CalcTangent(const Vector3& _axis, Vector3& _tangent1, Vector3& _tangent2);
}