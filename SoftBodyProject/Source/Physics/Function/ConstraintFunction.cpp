#include <algorithm>
#include <cmath>

#include "ConstraintFunction.h"

// 点拘束のヤコビアンと違反値の計算
void ConstraintFunction::CalcPointJacobianAndError(
	const Vector3& _positionA, const Vector3& _rA,
	const Vector3& _positionB, const Vector3& _rB,
	std::span<Constraint, 3> _output)
{
	// 差
	Vector3 diff{ _positionA - _positionB };
	// 差をそのまま拘束Cの結果とする
	_output[0].error = diff.x;


	// ヤコビアンの計算
	_output[0].jacobian[0] = Vector3::RIGHT;
	_output[0].jacobian[1] = Vector3::Cross(_rA, Vector3::RIGHT);
	_output[0].jacobian[2] = -Vector3::RIGHT;
	_output[0].jacobian[3] = -Vector3::Cross(_rB, Vector3::RIGHT);

	// 差をそのまま拘束Cの結果とする
	_output[1].error = diff.y;

	// ヤコビアンの計算
	_output[1].jacobian[0] = Vector3::UP;
	_output[1].jacobian[1] = Vector3::Cross(_rA, Vector3::UP);
	_output[1].jacobian[2] = -Vector3::UP;
	_output[1].jacobian[3] = -Vector3::Cross(_rB, Vector3::UP);

	// 差をそのまま拘束Cの結果とする
	_output[2].error = diff.z;


	// ヤコビアンの計算
	_output[2].jacobian[0] = Vector3::FORWARD;
	_output[2].jacobian[1] = Vector3::Cross(_rA, Vector3::FORWARD);
	_output[2].jacobian[2] = -Vector3::FORWARD;
	_output[2].jacobian[3] = -Vector3::Cross(_rB, Vector3::FORWARD);
}

// 距離拘束のヤコビアンと違反値の計算
void ConstraintFunction::CalcDistanceJacobianAndError(
	const float _distance,
	const Vector3& _positionA, const Vector3& _rA,
	const Vector3& _positionB, const Vector3& _rB,
	Constraint& _output)
{
	// 差
	Vector3 diff{ _positionA - _positionB };
	// 差をそのまま拘束Cの結果とする
	_output.error = diff.Length() - _distance;

	Vector3 normal;

	if (diff.LengthSqr() <= MathConstants::EPSILON)
	{
		normal = Vector3::UP;
	}
	else
	{
		normal = diff.Normalized();
	}


	// ヤコビアンの計算
	_output.jacobian[0] = normal;
	_output.jacobian[1] = Vector3::Cross(_rA, normal);
	_output.jacobian[2] = -normal;
	_output.jacobian[3] = -Vector3::Cross(_rB, normal);
}

// ヒンジ拘束のヤコビアンと違反値の計算
void ConstraintFunction::CalcHingeJacobianAndError(
	const Vector3& _positionA, const Vector3& _rA, const Vector3& _axisA,
	const Vector3& _positionB, const Vector3& _rB, const Vector3& _axisB,
	std::span<Constraint, 5> _output)
{
	Vector3 axisB{ _axisB };
	// ヒンジ軸を「向きのない直線」として扱う。
	// 逆向きなら、Aに近い向きへ反転する。
	if (Vector3::Dot(_axisA, _axisB) < 0.0f)
	{
		axisB = -_axisB;
	}

	// Row 1～3：アンカー位置を一致させる
	CalcPointJacobianAndError(
		_positionA, _rA,
		_positionB, _rB,
		_output.first<3>()
	);

	// ヒンジ軸に垂直な2方向
	Vector3 tangent1, tangent2;
	CalcTangent(_axisA, tangent1, tangent2);

	// A軸からB軸へ向かう軸ずれの回転ベクトル
	Vector3 crossAB{ Vector3::Cross(_axisA, axisB) };

	float sinTheta{ crossAB.Length() };

	float cosTheta{ Vector3::Dot(_axisA, axisB) };

	Vector3 axisError{ Vector3::ZERO };

	if (sinTheta > MathConstants::EPSILON)
	{
		float theta{ std::atan2(sinTheta, cosTheta) };

		// 方向 = crossAB / sinTheta
		// 長さ = theta
		axisError = crossAB * (theta / sinTheta);
	}

	// Row 4～5：軸に垂直な回転を止める
	CalcAxisJacobianAndError(tangent1, axisError, _output[3]);
	CalcAxisJacobianAndError(tangent2, axisError, _output[4]);
}

// 角度制限付き点拘束のヤコビアンと違反値の計算
void ConstraintFunction::CalcAngleLimitPointJacobianAndError(
	 const float _angleMax, const float _angleMin,
	const Vector3& _positionA, const Vector3& _rA, const Vector3& _dirA,
	const Vector3& _positionB, const Vector3& _rB, const Vector3& _dirB,
	std::span<Constraint, 4> _output)
{
	size_t count{ 3 };
	 // 通常の点拘束を入れる
	 CalcPointJacobianAndError(
		 _positionA, _rA,
		 _positionB, _rB,
		 _output.first<3>());

	 // 角度の制限を入れる
	 Vector3 crossAB{ Vector3::Cross(_dirA, _dirB) };

	 float sinTheta{ crossAB.Length() };
	 float cosTheta{ Vector3::Dot(_dirA, _dirB) };

	 float theta{ std::atan2(sinTheta, cosTheta) };

	 if (theta > _angleMax)
	 {
		 Vector3 n;

		 if (sinTheta > MathConstants::EPSILON)
		 {
			 n = crossAB / sinTheta;
		 }
		 else
		 {
			 Vector3 unused;
			 CalcTangent(_dirA, n, unused);
		 }

		 _output[3].error = theta - _angleMax;

		 _output[3].jacobian[0] = Vector3::ZERO; // linear A
		 _output[3].jacobian[1] = -n;            // angular A
		 _output[3].jacobian[2] = Vector3::ZERO; // linear B
		 _output[3].jacobian[3] = n;             // angular B
		 count++;

		 _output[3].isActive = true;
	 }
	 else if (theta < _angleMin)
	 {
		 Vector3 n;

		 if (sinTheta > MathConstants::EPSILON)
		 {
			 n = crossAB / sinTheta;
		 }
		 else
		 {
			 Vector3 unused;
			 CalcTangent(_dirA, n, unused);
		 }

		 _output[3].error = _angleMin - theta;

		 _output[3].jacobian[0] = Vector3::ZERO;
		 _output[3].jacobian[1] = n;
		 _output[3].jacobian[2] = Vector3::ZERO;
		 _output[3].jacobian[3] = -n;
		 count++;

		 _output[3].isActive = true;
	 }
	 else
	 {
		 _output[3].isActive = false;
	 }
}

// 角度制限付きヒンジ拘束のヤコビアンと違反値の計算
void ConstraintFunction::CalcAngleLimitHingeJacobianAndError(
	const float _angleMax, const float _angleMin,
	const Vector3& _positionA, const Vector3& _rA, const Vector3& _axisA, const Vector3 _referenceA,
	const Vector3& _positionB, const Vector3& _rB, const Vector3& _axisB, const Vector3 _referenceB,
	std::span<Constraint, 6> _output)
{
	// ヒンジ軸に垂直な2方向
	Vector3 tangent1, tangent2;
	CalcTangent(_axisA, tangent1, tangent2);

	// Row 1～3：アンカー位置
	CalcPointJacobianAndError(
		_positionA, _rA,
		_positionB, _rB,
		_output.first<3>());

	Vector3 axisB{ _axisB };

	if (Vector3::Dot(_axisA, axisB) < 0.0f)
	{
		axisB = -axisB;
	}

	// Row 4～5：ヒンジ軸
	Vector3 crossAB{ Vector3::Cross(_axisA, axisB) };
	float sinTheta{ crossAB.Length() };
	float cosTheta{ Vector3::Dot(_axisA, axisB) };
	Vector3 axisError{ Vector3::ZERO };

	if (sinTheta > MathConstants::EPSILON)
	{
		float theta{ std::atan2(sinTheta, cosTheta) };
		axisError = crossAB * (theta / sinTheta);
	}

	CalcAxisJacobianAndError(tangent1, axisError, _output[3]);
	CalcAxisJacobianAndError(tangent2, axisError, _output[4]);

	// Aの基準方向からBの基準方向への符号付き角度
	float sinAngle{ Vector3::Dot(_axisA, Vector3::Cross(_referenceA, _referenceB)) };
	float cosAngle{ Vector3::Dot(_referenceA, _referenceB) };
	float angle{ std::atan2(sinAngle, cosAngle) };

	size_t count{ 5 };
	// Row 6：範囲外の場合だけ片側角度拘束を作る
	if (angle > _angleMax)
	{
		_output[5].error = angle - _angleMax;
		_output[5].jacobian[0] = Vector3::ZERO;
		_output[5].jacobian[1] = -_axisA;
		_output[5].jacobian[2] = Vector3::ZERO;
		_output[5].jacobian[3] = _axisA;
		count++;

		_output[5].isActive = true;
	}
	else if (angle < _angleMin)
	{
		_output[5].error = _angleMin - angle;
		_output[5].jacobian[0] = Vector3::ZERO;
		_output[5].jacobian[1] = _axisA;
		_output[5].jacobian[2] = Vector3::ZERO;
		_output[5].jacobian[3] = -_axisA;
		count++;

		_output[5].isActive = true;
	}
	else
	{
		_output[5].isActive = false;
	}
}

// SwingTwist拘束のヤコビアンと違反値の計算
void ConstraintFunction::CalcLimitedBallJointJacobianAndError(
	const float _swingAngle, const float _twistAngleMax, const float _twistAngleMin,
	const Vector3& _positionA, const Vector3& _rA, const Vector3& _axisA, const Vector3 _referenceA,
	const Vector3& _positionB, const Vector3& _rB, const Vector3& _axisB, const Vector3 _referenceB,
	std::span<Constraint, 5> _output)
{
	// 通常の点拘束を入れる
	CalcPointJacobianAndError(
		_positionA, _rA,
		_positionB, _rB,
		_output.first<3>());

	// Swing角度の制限を入れる
	Vector3 crossAB{ Vector3::Cross(_axisA, _axisB) };

	float sinTheta{ crossAB.Length() };
	float cosTheta{ Vector3::Dot(_axisA, _axisB) };

	float theta{ std::atan2(sinTheta, cosTheta) };

	if (theta > _swingAngle)
	{
		Vector3 n;

		if (sinTheta > MathConstants::EPSILON)
		{
			n = crossAB / sinTheta;
		}
		else
		{
			Vector3 unused;
			CalcTangent(_axisA, n, unused);
		}

		_output[3].error = theta - _swingAngle;

		_output[3].jacobian[0] = Vector3::ZERO; // linear A
		_output[3].jacobian[1] = -n;            // angular A
		_output[3].jacobian[2] = Vector3::ZERO; // linear B
		_output[3].jacobian[3] = n;             // angular B

		_output[3].isActive = true;
	}
	else
	{
		_output[3].isActive = false;
	}

	// Twist角度の制限を入れる
	// Aの基準方向からBの基準方向への符号付き角度
	float sinAngle{ Vector3::Dot(_axisA, Vector3::Cross(_referenceA, _referenceB)) };
	float cosAngle{ Vector3::Dot(_referenceA, _referenceB) };
	float angle{ std::atan2(sinAngle, cosAngle) };

	// 範囲外の場合だけ片側角度拘束を作る
	if (angle > _twistAngleMax)
	{
		_output[4].error = angle - _twistAngleMax;
		_output[4].jacobian[0] = Vector3::ZERO;
		_output[4].jacobian[1] = -_axisA;
		_output[4].jacobian[2] = Vector3::ZERO;
		_output[4].jacobian[3] = _axisA;

		_output[4].isActive = true;
	}
	else if (angle < _twistAngleMin)
	{
		_output[4].error = _twistAngleMin - angle;
		_output[4].jacobian[0] = Vector3::ZERO;
		_output[4].jacobian[1] = _axisA;
		_output[4].jacobian[2] = Vector3::ZERO;
		_output[4].jacobian[3] = -_axisA;

		_output[4].isActive = true;
	}
	else
	{
		_output[4].isActive = false;
	}
}

// 関節駆動拘束のヤコビアンと違反値の計算
void ConstraintFunction::CalcJointDriveJacobianAndError(
	const Quaternion& _targetRelativeRotation,
	const Quaternion& _rotationA,
	const Quaternion& _rotationB,
	std::span<Constraint, 3> _output)
{
	// 相対姿勢を求める
	Quaternion relativeRotation{ _rotationA.Conjugate() * _rotationB };
	// C(Quaternion版)
	Quaternion errorRot{ relativeRotation * _targetRelativeRotation.Conjugate() };

	// このままでは使えないので、各方向にどれだけズレてるかに変更する

	Vector3 axis;
	float theta;
	errorRot.ToAxisAngle(axis, theta);

	// 角度が0に限りなく近いならやる意味もないでやんしょう
	if (theta <= MathConstants::EPSILON)
	{
		for (auto& output : _output)
		{
			output.isActive = false;
		}
		return;
	}

	for (auto& output : _output)
	{
		output.isActive = true;
	}

	Vector3 axisError{ axis * theta };

	Vector3 axisX{ _rotationA.Rotate(Vector3::RIGHT) };
	Vector3 axisY{ _rotationA.Rotate(Vector3::UP) };
	Vector3 axisZ{ _rotationA.Rotate(Vector3::FORWARD) };

	// ヤコビアンの計算
	_output[0].error = axisError.x;

	_output[0].jacobian[1] = -axisX;
	_output[0].jacobian[3] = axisX;

	_output[1].error = axisError.y;

	// ヤコビアンの計算
	_output[1].jacobian[1] = -axisY;
	_output[1].jacobian[3] = axisY;

	_output[2].error = axisError.z;

	// ヤコビアンの計算
	_output[2].jacobian[1] = -axisZ;
	_output[2].jacobian[3] = axisZ;
}

// ある点を線上の動きに制限する拘束のヤコビアンと違反値の計算
void ConstraintFunction::CalcPointOnLineJacobianAndError(
	const Quaternion& _rotationA, const Vector3& _positionA, const Vector3& _rA,
	const Vector3& _positionB, const Vector3& _rB,
	std::span<Constraint, 2> _output)
{
	Vector3 diffWorld{ _positionA - _positionB };

	Vector3 normals[2]{
		_rotationA.Rotate(Vector3::RIGHT),
		_rotationA.Rotate(Vector3::FORWARD)
	};

	for (int i = 0; i < 2; ++i)
	{
		const Vector3& n{ normals[i] };
		Constraint& row{ _output[i] };

		row.error = Vector3::Dot(diffWorld, n);

		row.jacobian[0] = n;
		row.jacobian[1] = Vector3::Cross(_rA - diffWorld, n);
		row.jacobian[2] = -n;
		row.jacobian[3] = -Vector3::Cross(_rB, n);
	}
}

// スライダー拘束のヤコビアンと違反値の計算
void ConstraintFunction::CalcSliderJacobianAndError(
	const Quaternion& _rotationA, const Vector3& _positionA, const Vector3& _rA,
	const Quaternion& _rotationB, const Vector3& _positionB, const Vector3& _rB,
	std::span<Constraint, 5> _output)
{
	// ある点を線上の動きに制限する
	CalcPointOnLineJacobianAndError(
		_rotationA, _positionA, _rA,
		_positionB, _rB,
		_output.first<2>());

	// 姿勢を軸に合わせる
	CalcJointDriveJacobianAndError(
		Quaternion::IDENTITY,
		_rotationA,
		_rotationB,
		_output.subspan<2, 3>()
	);
}

// 距離制限のある、ある点を線上の動きに制限する拘束のヤコビアンと違反値の計算
void ConstraintFunction::CalcLimitedPointOnLineJacobianAndError(
	const float _distance,
	const Quaternion& _rotationA, const Vector3& _positionA, const Vector3& _rA,
	const Vector3& _positionB, const Vector3& _rB,
	std::span<Constraint, 3> _output)
{
	Vector3 diffWorld{ _positionA - _positionB };

	Vector3 normals[3]{
		_rotationA.Rotate(Vector3::RIGHT),
		_rotationA.Rotate(Vector3::UP),
		_rotationA.Rotate(Vector3::FORWARD),
	};

	float maxDistances[3]{ 0.0f,_distance,0.0f };

	for (int i = 0; i < 3; i++)
	{
		const Vector3& n{ normals[i] };
		Constraint& row{ _output[i] };
		float maxDistance{ maxDistances[i] };

		row.error = Vector3::Dot(diffWorld, n);

		if (row.error <= maxDistances[i])
		{
			row.isActive = false;
			continue;
			
		}

		row.isActive = true;
		row.error -= maxDistances[i];

		row.jacobian[0] = n;
		row.jacobian[1] = Vector3::Cross(_rA - diffWorld, n);
		row.jacobian[2] = -n;
		row.jacobian[3] = -Vector3::Cross(_rB, n);
	}
}

// 距離制限のある、スライダー拘束のヤコビアンと違反値の計算
void ConstraintFunction::CalcLimitedSliderJacobianAndError(
	const float _distance,
	const Quaternion& _rotationA, const Vector3& _positionA, const Vector3& _rA,
	const Quaternion& _rotationB, const Vector3& _positionB, const Vector3& _rB,
	std::span<Constraint, 6> _output)
{
	// 距離制限付きで、ある点を線上に拘束する
	CalcLimitedPointOnLineJacobianAndError(
		_distance,
		_rotationA, _positionA, _rA,
		_positionB, _rB,
		_output.first<3>()
	);

	// 姿勢を軸に合わせる
	CalcJointDriveJacobianAndError(
		Quaternion::IDENTITY,
		_rotationA,
		_rotationB,
		_output.subspan<3, 3>()
	);
}

// 軸合わせヤコビアンと違反値の計算
void ConstraintFunction::CalcAxisJacobianAndError(const Vector3& _tangent, const Vector3& _axisError, Constraint& _output)
{
	// 軸ずれをtangent方向へ射影
	_output.error = Vector3::Dot(_axisError, _tangent);

	// Jω =
	// -tangent・ωA + tangent・ωB
	_output.jacobian[0] = Vector3::ZERO;
	_output.jacobian[1] = -_tangent;
	_output.jacobian[2] = Vector3::ZERO;
	_output.jacobian[3] = _tangent;
}

// 軸に対して垂直なベクトル2つを生成する関数
void ConstraintFunction::CalcTangent(const Vector3& _axis, Vector3& _tangent1, Vector3& _tangent2)
{
	// 平行になりずらい方向を選びそれを元にタンジェントを生成
	int seedAxisIndex{ 0 };

	for (int i{ 1 }; i < 3; ++i)
	{
		if (std::abs(_axis[i]) < std::abs(_axis[seedAxisIndex]))
		{
			seedAxisIndex = i;
		}
	}

	Vector3 seed{ Vector3::ZERO };
	seed[seedAxisIndex] = 1.0f;

	_tangent1 = Vector3::Cross(_axis, seed).Normalized();
	_tangent2 = Vector3::Cross(_axis, _tangent1).Normalized();
}