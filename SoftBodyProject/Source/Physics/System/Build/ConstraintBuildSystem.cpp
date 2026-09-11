#include <algorithm>

#include "TimeManager.h"

#include "ConstraintBuildSystem.h"

void ConstraintBuildSystem::FixedUpdate(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer)
{
	BuildPointConstraint(_constraintStorage, _solverBodyBuffer, _constraintBuffer);
	BuildDistanceConstraint(_constraintStorage, _solverBodyBuffer, _constraintBuffer);
	BuildHingeConstraint(_constraintStorage, _solverBodyBuffer, _constraintBuffer);
	BuildAngleLimitPointConstraint(_constraintStorage, _solverBodyBuffer, _constraintBuffer);
	BuildAngleLimitHingeConstraint(_constraintStorage, _solverBodyBuffer, _constraintBuffer);
}

// 点拘束の解く用の拘束構造体を作る
void ConstraintBuildSystem::BuildPointConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer)
{
	// 拘束が存在するかチェック
	if (_constraintStorage->CountPointConstraint() <= 0)
	{
		return;
	}
	for (auto& pointConstraint : _constraintStorage->GetPointConstraintRange())
	{
		// ポイントが2つ以上じゃないと拘束なんて発生しない
		if (pointConstraint.endPoints.size() <= 1)
		{
			continue;
		}

		// 基準点となるボディから位置を持ってくる。
		uint32_t basePointIndex{ _solverBodyBuffer->GetIndex(pointConstraint.endPoints[0].transformID) };
		const SolverBody& solverBodyBase{ _solverBodyBuffer->Get(basePointIndex) };
		Vector3 basePoint{ solverBodyBase.position + solverBodyBase.rotation.Rotate(pointConstraint.endPoints[0].localPoint) };

		for (int i{ 1 }; i < pointConstraint.endPoints.size(); i++)
		{
			Constraint constraint;

			// 対象の位置を取得
			uint32_t pointIndex{ _solverBodyBuffer->GetIndex(pointConstraint.endPoints[i].transformID) };
			const SolverBody& solverBody{ _solverBodyBuffer->Get(pointIndex) };
			Vector3 point{ solverBody.position + solverBody.rotation.Rotate(pointConstraint.endPoints[i].localPoint) };

			constraint.solverBodyAIndex = basePointIndex;
			constraint.solverBodyBIndex = pointIndex;

			Vector3 rA = basePoint - solverBodyBase.position;
			Vector3	rB = point - solverBody.position;

			AddPointConstraint(
				constraint, pointConstraint.tuning,
				rA, basePoint,
				rB, point,
				_constraintBuffer
			);
		}
	}
}

// 距離拘束の解く用の拘束構造体を作る
void ConstraintBuildSystem::BuildDistanceConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer)
{
	// 拘束が存在するかチェック
	if (_constraintStorage->CountDistanceConstraint() <= 0)
	{
		return;
	}
	for (auto& distanceConstraint : _constraintStorage->GetDistanceConstraintRange())
	{
		// ポイントが2つ以上じゃないと拘束なんて発生しない
		if (distanceConstraint.endPoints.size() <= 1)
		{
			continue;
		}

		// 基準点となるボディから位置を持ってくる。
		uint32_t basePointIndex{ _solverBodyBuffer->GetIndex(distanceConstraint.endPoints[0].transformID) };
		const SolverBody& solverBodyBase{ _solverBodyBuffer->Get(basePointIndex) };
		Vector3 basePoint{ solverBodyBase.position + solverBodyBase.rotation.Rotate(distanceConstraint.endPoints[0].localPoint) };

		for (int i{ 1 }; i < distanceConstraint.endPoints.size(); i++)
		{
			Constraint constraint;

			// 対象の位置を取得
			uint32_t pointIndex{ _solverBodyBuffer->GetIndex(distanceConstraint.endPoints[i].transformID) };
			const SolverBody& solverBody{ _solverBodyBuffer->Get(pointIndex) };
			Vector3 point{ solverBody.position + solverBody.rotation.Rotate(distanceConstraint.endPoints[i].localPoint) };

			constraint.solverBodyAIndex = basePointIndex;
			constraint.solverBodyBIndex = pointIndex;

			Vector3 rA = basePoint - solverBodyBase.position;
			Vector3	rB = point - solverBody.position;

			// 差
			Vector3 diff{ basePoint - point };
			// 差をそのまま拘束Cの結果とする
			constraint.error = diff.Length() - distanceConstraint.distance;

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
			constraint.jacobian[0] = normal;
			constraint.jacobian[1] = Vector3::Cross(rA, normal);
			constraint.jacobian[2] = -normal;
			constraint.jacobian[3] = -Vector3::Cross(rB, normal);

			MakeConstraintInfo(constraint, distanceConstraint.tuning);

			// 拘束として追加
			_constraintBuffer->Add(constraint);
		}
	}
}

// ヒンジ拘束の解く用の拘束構造体を作る
void ConstraintBuildSystem::BuildHingeConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer)
{
	// 拘束が存在するかチェック
	if (_constraintStorage->CountHingeConstraint() <= 0)
	{
		return;
	}

	for (auto& hingeConstraint : _constraintStorage->EditHingeConstraintRange())
	{
		// 対象がいないとダメ
		if (hingeConstraint.directionEndPoints.size() < 1)
		{
			continue;
		}

		// 基準点となるボディから位置を持ってくる。
		uint32_t basePointIndex{ _solverBodyBuffer->GetIndex(hingeConstraint.ownerEndPoint.transformID) };
		const SolverBody& solverBodyBase{ _solverBodyBuffer->Get(basePointIndex) };
		Vector3 basePoint{ solverBodyBase.position + solverBodyBase.rotation.Rotate(hingeConstraint.ownerEndPoint.localPosition) };

		Vector3 axis{ solverBodyBase.rotation
			.Rotate(hingeConstraint.ownerEndPoint.localDirection)
			.Normalized() };

		for (auto& directionEndPoint : hingeConstraint.directionEndPoints)
		{
			// 対象の位置を取得
			uint32_t pointIndex{ _solverBodyBuffer->GetIndex(directionEndPoint.transformID) };
			const SolverBody& solverBody{ _solverBodyBuffer->Get(pointIndex) };
			Vector3 point{ solverBody.position + solverBody.rotation.Rotate(directionEndPoint.localPosition) };

			Vector3 rA{ basePoint - solverBodyBase.position };
			Vector3 rB{ point - solverBody.position };

			// B側ヒンジ軸
			Vector3 axisB{ solverBody.rotation.Rotate(directionEndPoint.localDirection) };

			// 不正なヒンジ軸
			if (axisB.LengthSqr() <= MathConstants::EPSILON)
			{
				continue;
			}

			axisB.Normalize();

			// ヒンジ軸を「向きのない直線」として扱う。
			// 逆向きなら、Aに近い向きへ反転する。
			if (Vector3::Dot(axis, axisB) < 0.0f)
			{
				axisB = -axisB;
			}

			Constraint constraint;

			constraint.solverBodyAIndex = basePointIndex;
			constraint.solverBodyBIndex = pointIndex;

			// Row 1～3：アンカー位置を一致させる
			AddPointConstraint(
				constraint, hingeConstraint.positionTuning,
				rA, basePoint,
				rB, point,
				_constraintBuffer
			);

			// axisと平行になりにくい基準軸を選ぶ。
			float rightDot{ Vector3::Dot(axis, Vector3::RIGHT) };

			Vector3 seed{ rightDot * rightDot < 0.81f ? Vector3::RIGHT : Vector3::UP };

			Vector3 tangent1{ Vector3::Cross(axis, seed).Normalized() };

			Vector3 tangent2{ Vector3::Cross(axis, tangent1).Normalized() };

			// A軸からB軸へ向かう軸ずれの回転ベクトル
			Vector3 crossAB{ Vector3::Cross(axis, axisB) };

			float sinTheta{ crossAB.Length() };

			float cosTheta{ std::clamp(Vector3::Dot(axis, axisB),-1.0f,1.0f) };

			Vector3 axisError{ Vector3::ZERO };

			if (sinTheta > MathConstants::EPSILON)
			{
				float theta{ std::atan2(sinTheta, cosTheta) };

				// 方向 = crossAB / sinTheta
				// 長さ = theta
				axisError = crossAB * (theta / sinTheta);
			}

			// Row 4～5：軸に垂直な回転を止める
			AddAxisConstraint(
				constraint, hingeConstraint.angularTuning,
				tangent1, axisError,
				_constraintBuffer);
			AddAxisConstraint(
				constraint, hingeConstraint.angularTuning,
				tangent2, axisError,
				_constraintBuffer);
		}
	}
}

// 角度制限付き点拘束の解く用の拘束構造体を作る
void ConstraintBuildSystem::BuildAngleLimitPointConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer)
{
	// 拘束が存在するかチェック
	if (_constraintStorage->CountAngleLimitPointConstraint() <= 0)
	{
		return;
	}

	for (auto& angleLimitPointConstraint : _constraintStorage->EditAngleLimitPointConstraintRange())
	{
		// ポイントが2つ以上じゃないと拘束なんて発生しない
		if (angleLimitPointConstraint.directionEndPoints.size() <= 1)
		{
			continue;
		}

		// 基準点となるボディから位置を持ってくる。
		uint32_t basePointIndex{ _solverBodyBuffer->GetIndex(angleLimitPointConstraint.directionEndPoints[0].transformID) };
		const SolverBody& solverBodyBase{ _solverBodyBuffer->Get(basePointIndex) };
		Vector3 basePoint{ solverBodyBase.position + solverBodyBase.rotation.Rotate(angleLimitPointConstraint.directionEndPoints[0].localPosition) };
		Vector3 baseDir{ solverBodyBase.rotation.Rotate(angleLimitPointConstraint.directionEndPoints[0].localDirection) };

		for (int i{ 1 }; i < angleLimitPointConstraint.directionEndPoints.size(); i++)
		{
			Constraint constraint;

			// 対象の位置を取得
			uint32_t pointIndex{ _solverBodyBuffer->GetIndex(angleLimitPointConstraint.directionEndPoints[i].transformID) };
			const SolverBody& solverBody{ _solverBodyBuffer->Get(pointIndex) };
			Vector3 point{ solverBody.position + solverBody.rotation.Rotate(angleLimitPointConstraint.directionEndPoints[i].localPosition) };
			Vector3 dir{ solverBody.rotation.Rotate(angleLimitPointConstraint.directionEndPoints[i].localDirection) };

			constraint.solverBodyAIndex = basePointIndex;
			constraint.solverBodyBIndex = pointIndex;

			Vector3 rA = basePoint - solverBodyBase.position;
			Vector3	rB = point - solverBody.position;

			// 通常の点拘束を入れる
			AddPointConstraint(
				constraint, angleLimitPointConstraint.tuning,
				rA, basePoint,
				rB, point,
				_constraintBuffer
			);

			// 角度の制限を入れる
			Vector3 crossAB{ Vector3::Cross(baseDir, dir) };

			float sinTheta{ crossAB.Length() };
			float cosTheta = std::clamp(Vector3::Dot(baseDir, dir), -1.0f, 1.0f);

			float theta{ std::atan2(sinTheta, cosTheta) };

			if (theta > angleLimitPointConstraint.angleMax)
			{
				Vector3 n{ crossAB / sinTheta };

				constraint.error = theta - angleLimitPointConstraint.angleMax;

				constraint.jacobian[0] = Vector3::ZERO; // linear A
				constraint.jacobian[1] = -n;            // angular A
				constraint.jacobian[2] = Vector3::ZERO; // linear B
				constraint.jacobian[3] = n;            // angular B

				MakeConstraintInfo(constraint, angleLimitPointConstraint.tuning);

				constraint.minLambda = 0.0f;
				constraint.maxLambda = angleLimitPointConstraint.tuning.maxForce * TimeManager::GetFixedDeltaTime();

				// 拘束として追加
				_constraintBuffer->Add(constraint);
			}
			else if (theta < angleLimitPointConstraint.angleMin)
			{
				Vector3 n{ crossAB / sinTheta };

				constraint.error = angleLimitPointConstraint.angleMin - theta;

				constraint.jacobian[0] = Vector3::ZERO;
				constraint.jacobian[1] = n;
				constraint.jacobian[2] = Vector3::ZERO;
				constraint.jacobian[3] = -n;

				MakeConstraintInfo(constraint, angleLimitPointConstraint.tuning);

				constraint.minLambda = 0.0f;
				constraint.maxLambda = angleLimitPointConstraint.tuning.maxForce * TimeManager::GetFixedDeltaTime();

				// 拘束として追加
				_constraintBuffer->Add(constraint);
			}
			// 範囲内なら角度Constraintを生成しない
		}
	}
}

// 角度制限付きヒンジ拘束の解く用の拘束構造体を作る
void ConstraintBuildSystem::BuildAngleLimitHingeConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer)
{
	// 拘束が存在するかチェック
	if (_constraintStorage->CountAngleLimitHingeConstraint() <= 0)
	{
		return;
	}

	for (auto& angleLimitHingeConstraint : _constraintStorage->EditAngleLimitHingeConstraintRange())
	{
		// 対象がいないとダメ
		if (angleLimitHingeConstraint.endPoint.size() < 1)
		{
			continue;
		}

		// 基準側
		uint32_t basePointIndex{ _solverBodyBuffer->GetIndex(angleLimitHingeConstraint.ownerEndPoint.transformID) };
		const SolverBody& solverBodyBase{ _solverBodyBuffer->Get(basePointIndex) };
		Vector3 basePoint{ solverBodyBase.position + solverBodyBase.rotation.Rotate(angleLimitHingeConstraint.ownerEndPoint.localPosition) };
		Vector3 axis{ solverBodyBase.rotation.Rotate(angleLimitHingeConstraint.ownerEndPoint.localAxis) };

		if (axis.LengthSqr() <= MathConstants::EPSILON)
		{
			continue;
		}

		axis.Normalize();

		// A側の角度0基準方向
		Vector3 referenceA{ solverBodyBase.rotation.Rotate(angleLimitHingeConstraint.ownerEndPoint.localReferenceDirection) };
		referenceA = referenceA - axis * Vector3::Dot(axis, referenceA);

		bool canMeasureAngle{ referenceA.LengthSqr() > MathConstants::EPSILON };

		if (canMeasureAngle)
		{
			referenceA.Normalize();
		}

		// ヒンジ軸に垂直な2方向
		float rightDot{ Vector3::Dot(axis, Vector3::RIGHT) };
		Vector3 seed{ rightDot * rightDot < 0.81f ? Vector3::RIGHT : Vector3::UP };
		Vector3 tangent1{ Vector3::Cross(axis, seed).Normalized() };
		Vector3 tangent2{ Vector3::Cross(axis, tangent1).Normalized() };

		for (auto& endPoint : angleLimitHingeConstraint.endPoint)
		{
			// 対象側
			uint32_t pointIndex{ _solverBodyBuffer->GetIndex(endPoint.transformID) };
			const SolverBody& solverBody{ _solverBodyBuffer->Get(pointIndex) };
			Vector3 point{ solverBody.position + solverBody.rotation.Rotate(endPoint.localPosition) };
			Vector3 axisB{ solverBody.rotation.Rotate(endPoint.localAxis) };

			if (axisB.LengthSqr() <= MathConstants::EPSILON)
			{
				continue;
			}

			axisB.Normalize();

			// 通常Hingeと同じく軸を直線として扱う
			if (Vector3::Dot(axis, axisB) < 0.0f)
			{
				axisB = -axisB;
			}

			Vector3 rA{ basePoint - solverBodyBase.position };
			Vector3 rB{ point - solverBody.position };

			Constraint constraint;
			constraint.solverBodyAIndex = basePointIndex;
			constraint.solverBodyBIndex = pointIndex;

			// Row 1～3：アンカー位置
			AddPointConstraint(constraint, angleLimitHingeConstraint.positionTuning, rA, basePoint, rB, point, _constraintBuffer);

			// Row 4～5：ヒンジ軸
			Vector3 crossAB{ Vector3::Cross(axis, axisB) };
			float sinTheta{ crossAB.Length() };
			float cosTheta{ std::clamp(Vector3::Dot(axis, axisB), -1.0f, 1.0f) };
			Vector3 axisError{ Vector3::ZERO };

			if (sinTheta > MathConstants::EPSILON)
			{
				float theta{ std::atan2(sinTheta, cosTheta) };
				axisError = crossAB * (theta / sinTheta);
			}

			AddAxisConstraint(constraint, angleLimitHingeConstraint.angularTuning, tangent1, axisError, _constraintBuffer);
			AddAxisConstraint(constraint, angleLimitHingeConstraint.angularTuning, tangent2, axisError, _constraintBuffer);

			// 基準方向が作れない場合は通常Hingeとしてだけ働かせる
			if (!canMeasureAngle)
			{
				continue;
			}

			// B側の基準方向をA側ヒンジ軸の平面へ射影
			Vector3 referenceB{ solverBody.rotation.Rotate(endPoint.localReferenceDirection) };
			referenceB = referenceB - axis * Vector3::Dot(axis, referenceB);

			if (referenceB.LengthSqr() <= MathConstants::EPSILON)
			{
				continue;
			}

			referenceB.Normalize();

			// Aの基準方向からBの基準方向への符号付き角度
			float sinAngle{ Vector3::Dot(axis, Vector3::Cross(referenceA, referenceB)) };
			float cosAngle{ std::clamp(Vector3::Dot(referenceA, referenceB), -1.0f, 1.0f) };
			float angle{ std::atan2(sinAngle, cosAngle) };

			// Row 6：範囲外の場合だけ片側角度拘束を作る
			if (angle > angleLimitHingeConstraint.angleMax)
			{
				constraint.error = angle - angleLimitHingeConstraint.angleMax;
				constraint.jacobian[0] = Vector3::ZERO;
				constraint.jacobian[1] = -axis;
				constraint.jacobian[2] = Vector3::ZERO;
				constraint.jacobian[3] = axis;
			}
			else if (angle < angleLimitHingeConstraint.angleMin)
			{
				constraint.error = angleLimitHingeConstraint.angleMin - angle;
				constraint.jacobian[0] = Vector3::ZERO;
				constraint.jacobian[1] = axis;
				constraint.jacobian[2] = Vector3::ZERO;
				constraint.jacobian[3] = -axis;
			}
			else
			{
				continue;
			}

			MakeConstraintInfo(constraint, angleLimitHingeConstraint.angularTuning);

			// Limitなので片側拘束
			constraint.minLambda = 0.0f;
			constraint.maxLambda = angleLimitHingeConstraint.angularTuning.maxForce * TimeManager::GetFixedDeltaTime();

			_constraintBuffer->Add(constraint);
		}
	}
}


void ConstraintBuildSystem::MakeConstraintInfo(Constraint& _constraint, const ConstraintTuning& _tuning)
{
	float deltaTime{ TimeManager::GetFixedDeltaTime() };

	float denominator = _tuning.damping + deltaTime * _tuning.stiffness;

	_constraint.softness = 1.0f / denominator;

	float erp{ deltaTime * _tuning.stiffness / denominator };

	_constraint.bias = erp / deltaTime * _constraint.error;

	_constraint.targetVelocity = 0.0f;

	_constraint.minLambda = -_tuning.maxForce * deltaTime;
	_constraint.maxLambda = _tuning.maxForce * deltaTime;
}

// 点拘束を情報からソルバ用拘束に変換してバッファに入れる
void ConstraintBuildSystem::AddPointConstraint(
	Constraint _constraint, const ConstraintTuning& _tuning,
	const Vector3& _rA, const Vector3& _pointA,
	const Vector3& _rB, const Vector3& _pointB,
	ConstraintBuffer* _constraintBuffer)
{
	// 差
	Vector3 diff{ _pointA - _pointB };
	// 差をそのまま拘束Cの結果とする
	_constraint.error = diff.x;


	// ヤコビアンの計算
	_constraint.jacobian[0] = Vector3::RIGHT;
	_constraint.jacobian[1] = Vector3::Cross(_rA, Vector3::RIGHT);
	_constraint.jacobian[2] = -Vector3::RIGHT;
	_constraint.jacobian[3] = -Vector3::Cross(_rB, Vector3::RIGHT);

	MakeConstraintInfo(_constraint, _tuning);

	// 拘束として追加
	_constraintBuffer->Add(_constraint);

	// 差をそのまま拘束Cの結果とする
	_constraint.error = diff.y;


	// ヤコビアンの計算
	_constraint.jacobian[0] = Vector3::UP;
	_constraint.jacobian[1] = Vector3::Cross(_rA, Vector3::UP);
	_constraint.jacobian[2] = -Vector3::UP;
	_constraint.jacobian[3] = -Vector3::Cross(_rB, Vector3::UP);

	MakeConstraintInfo(_constraint, _tuning);

	// 拘束として追加
	_constraintBuffer->Add(_constraint);

	// 差をそのまま拘束Cの結果とする
	_constraint.error = diff.z;


	// ヤコビアンの計算
	_constraint.jacobian[0] = Vector3::FORWARD;
	_constraint.jacobian[1] = Vector3::Cross(_rA, Vector3::FORWARD);
	_constraint.jacobian[2] = -Vector3::FORWARD;
	_constraint.jacobian[3] = -Vector3::Cross(_rB, Vector3::FORWARD);

	MakeConstraintInfo(_constraint, _tuning);

	// 拘束として追加
	_constraintBuffer->Add(_constraint);
}

void ConstraintBuildSystem::AddAxisConstraint(
	Constraint _constraint, const ConstraintTuning& _tuning,
	const Vector3& _tangent, const Vector3& _axisError,
	ConstraintBuffer* _constraintBuffer)
{
	// 軸ずれをtangent方向へ射影
	_constraint.error = Vector3::Dot(_axisError, _tangent);

	// Jω =
	// -tangent・ωA + tangent・ωB
	_constraint.jacobian[0] = Vector3::ZERO;
	_constraint.jacobian[1] = -_tangent;
	_constraint.jacobian[2] = Vector3::ZERO;
	_constraint.jacobian[3] = _tangent;

	MakeConstraintInfo(_constraint, _tuning);

	_constraintBuffer->Add(_constraint);
};