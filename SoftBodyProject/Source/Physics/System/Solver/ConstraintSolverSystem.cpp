#include <algorithm>

#include "TimeManager.h"

#include "BodyStorage.h"

#include "ConstraintSolverSystem.h"

#include "ConstraintFunction.h"

void ConstraintSolverSystem::Solve(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer)
{
	size_t batchCount{ 0 };
	// 点拘束の解く用の拘束のヤコビアンと違反値の再計算
	SolvePointConstraint(_constraintStorage, _solverBodyBuffer, _constraintBuffer, batchCount);
	// 距離拘束の解く用の拘束のヤコビアンと違反値の再計算
	SolveDistanceConstraint(_constraintStorage, _solverBodyBuffer, _constraintBuffer, batchCount);
	// ヒンジ拘束の解く用の拘束のヤコビアンと違反値の再計算
	SolveHingeConstraint(_constraintStorage, _solverBodyBuffer, _constraintBuffer, batchCount);
	// 角度制限付き点拘束の解く用の拘束のヤコビアンと違反値の再計算
	SolveAngleLimitPointConstraint(_constraintStorage, _solverBodyBuffer, _constraintBuffer, batchCount);
	// 角度制限付きヒンジ拘束の解く用の拘束のヤコビアンと違反値の再計算
	SolveAngleLimitHingeConstraint(_constraintStorage, _solverBodyBuffer, _constraintBuffer, batchCount);
	// SwingTwist拘束の解く用の拘束のヤコビアンと違反値の再計算
	SolveLimitedBallJointConstraint(_constraintStorage, _solverBodyBuffer, _constraintBuffer, batchCount);
	// 関節駆動拘束の解く用の拘束のヤコビアンと違反値の再計算
	SolveJointDriveConstraint(_constraintStorage, _solverBodyBuffer, _constraintBuffer, batchCount);
}

// 速度再計算
void ConstraintSolverSystem::ReCalcVelocity(SolverBodyBuffer* _solverBodyBuffer)
{
	for (auto& body : _solverBodyBuffer->EditAll())
	{
		if (body.inverseMass != 0.0f)
		{
			// 変化した速度から位置を再計算
			// 計算後位置 - 計算前位置を移動距離として速度を計算
			body.velocity = (body.position - body.pastPos) / TimeManager::GetFixedDeltaTime();
			body.velocity = SIMDVectorMath::Mul(body.velocity, body.linearFactor);

			// 変化前姿勢と変化後姿勢の差分から角速度を計算
			Quaternion deltaRot{ body.rotation * body.pastRot.Conjugate() };
			// 軸と角度に分解
			Vector3 axis;
			float theta;
			deltaRot.ToAxisAngle(axis, theta);
			// 時間ステップで割って角速度にする
			theta /= TimeManager::GetFixedDeltaTime();
			body.angularVelocity = SIMDVectorMath::Mul(axis * theta, body.angularFactor);
		}
	}
}

// 拘束生成
void ConstraintSolverSystem::Build(
	ConstraintBuffer* _constraintBuffer, const ConstraintTuning& _tuning,
	float _weightA, float _weightB, size_t _buildSize, size_t _batchCount)
{
	if (_batchCount != _constraintBuffer->BatchCount())
	{
		return;
	}

	ConstraintRowBatch batch;
	batch.firstRow = _constraintBuffer->GetSize();
	batch.rowCount = _buildSize;

	_constraintBuffer->AddBatch(batch);

	for (size_t i{ 0 }; i < _buildSize; i++)
	{
		Constraint constraint;

		constraint.weightA = _weightA;
		constraint.weightB = _weightB;

		MakeConstraintInfo(constraint, _tuning);

		_constraintBuffer->Add(constraint);
	}
}

// 拘束生成
void ConstraintSolverSystem::Build(ConstraintBuffer* _constraintBuffer,
	const ConstraintTuning& _positionTuning, size_t _positionSize,
	const ConstraintTuning& _angulerTuning, size_t _angulerSize,
	float _weightA, float _weightB,
	size_t _batchCount)
{
	if (_batchCount != _constraintBuffer->BatchCount())
	{
		return;
	}

	ConstraintRowBatch batch;
	batch.firstRow = _constraintBuffer->GetSize();
	batch.rowCount = _positionSize + _angulerSize;

	_constraintBuffer->AddBatch(batch);

	for (size_t i{ 0 }; i < _positionSize; i++)
	{
		Constraint constraint;

		constraint.weightA = _weightA;
		constraint.weightB = _weightB;

		MakeConstraintInfo(constraint, _positionTuning);

		_constraintBuffer->Add(constraint);
	}
	for (size_t i{ 0 }; i < _angulerSize; i++)
	{
		Constraint constraint;

		constraint.weightA = _weightA;
		constraint.weightB = _weightB;

		MakeConstraintInfo(constraint, _angulerTuning);

		_constraintBuffer->Add(constraint);
	}
}

void ConstraintSolverSystem::MakeConstraintInfo(Constraint& _constraint, const ConstraintTuning& _tuning)
{
	float deltaTime{ TimeManager::GetFixedDeltaTime() };

	_constraint.timeStepAdjustedCompliance = _tuning.compliance / (deltaTime * deltaTime);

	_constraint.minLambda = -_tuning.maxForce * deltaTime * deltaTime;
	_constraint.maxLambda = _tuning.maxForce * deltaTime * deltaTime;
}

// XPBD法による位置解消関数
void ConstraintSolverSystem::SolveRow(SolverBody& _solverBodyA, SolverBody& _solverBodyB, Constraint& _constraint)
{
	if (!_constraint.isActive)
	{
		return;
	}
	// 質量から両者がBodyを持っているかの判定をする(どちらかがBodyを持っているなら合計は0じゃないはず)
	float totalInvMass{ _solverBodyA.inverseMass * _constraint.weightA + _solverBodyB.inverseMass * _constraint.weightB };
	if (totalInvMass <= 0)
	{
		return;
	}

	// 慣性テンソル求める
	Matrix4x4 rotMat{ MatGenerateFunc::Rotate(_solverBodyA.rotation) };
	Matrix4x4 worldInverseInertiaTnesorA{ rotMat * _solverBodyA.localInverseInertiaTensor * rotMat.Transposed() };

	rotMat = MatGenerateFunc::Rotate(_solverBodyB.rotation);
	Matrix4x4 worldInverseInertiaTnesorB{ rotMat * _solverBodyB.localInverseInertiaTensor * rotMat.Transposed() };

	// Factorを計算した各種速度計算
	Vector3 linearResponseA{
		BodyStorage::ApplyLinearInverseMass(
			_constraint.jacobian[0],
			_solverBodyA.inverseMass,
			_solverBodyA.linearFactor)
	};

	Vector3 angularResponseA{
		BodyStorage::ApplyAngularInverseInertia(
			worldInverseInertiaTnesorA,
			_constraint.jacobian[1],
			_solverBodyA.angularFactor)
	};

	Vector3 linearResponseB{
		BodyStorage::ApplyLinearInverseMass(
			_constraint.jacobian[2],
			_solverBodyB.inverseMass,
			_solverBodyB.linearFactor)
	};

	Vector3 angularResponseB{
		BodyStorage::ApplyAngularInverseInertia(
			worldInverseInertiaTnesorB,
			_constraint.jacobian[3],
			_solverBodyB.angularFactor)
	};

	// 質量と慣性テンソルが速度に影響する度合い
	float effectiveMass{
		Vector3::Dot(_constraint.jacobian[0], linearResponseA) +
		Vector3::Dot(_constraint.jacobian[1], angularResponseA) +
		Vector3::Dot(_constraint.jacobian[2], linearResponseB) +
		Vector3::Dot(_constraint.jacobian[3], angularResponseB)
	};

	if (effectiveMass < MathConstants::EPSILON)
	{
		return;
	}

	// λ計算
	float oldLambda{ _constraint.accumulatedLambda };

	float deltaLambda{
		(_constraint.error - _constraint.timeStepAdjustedCompliance * oldLambda) /
		(effectiveMass + _constraint.timeStepAdjustedCompliance) };

	// 合計値を計算
	_constraint.accumulatedLambda = std::clamp(
		oldLambda + deltaLambda,
		_constraint.minLambda,
		_constraint.maxLambda);

	float applyLambda{ _constraint.accumulatedLambda - oldLambda };

	// Aの位置/姿勢制御
	_solverBodyA.position -= linearResponseA * applyLambda;
	Vector3 angVec{ angularResponseA * applyLambda };
	Quaternion rotOmega{ Quaternion::AngleAxis(angVec.Length(), -angVec) };
	_solverBodyA.rotation = rotOmega * _solverBodyA.rotation;

	// Bの位置/姿勢制御
	_solverBodyB.position -= linearResponseB * applyLambda;
	angVec = angularResponseB * applyLambda;
	rotOmega = Quaternion::AngleAxis(angVec.Length(), -angVec);
	_solverBodyB.rotation = rotOmega * _solverBodyB.rotation;
}

// 点拘束の解く用の拘束のヤコビアンと違反値の再計算
void ConstraintSolverSystem::SolvePointConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer, size_t& _batchCount)
{
	// 拘束が存在するかチェック
	if (_constraintStorage->CountPointConstraint() <= 0)
	{
		return;
	}

	for (auto id : _constraintStorage->GetPointConstraintIDRange())
	{
		const PointConstraint& pointConstraint{ _constraintStorage->GetPointConstraint(id) };
		// ポイントが2つ以上じゃないと拘束なんて発生しない
		if (pointConstraint.endPoints.size() <= 0)
		{
			continue;
		}

		// 基準点となるボディから位置を持ってくる。
		uint32_t basePointIndex{ _solverBodyBuffer->GetIndex(pointConstraint.ownerEndPoint.transformID) };
		SolverBody& solverBodyBase{ _solverBodyBuffer->Edit(basePointIndex) };
		Vector3 basePoint{ solverBodyBase.position + solverBodyBase.rotation.Rotate(pointConstraint.ownerEndPoint.localPosition) };

		for (int i{ 0 }; i < pointConstraint.endPoints.size(); i++)
		{
			// 対象の位置を取得
			uint32_t pointIndex{ _solverBodyBuffer->GetIndex(pointConstraint.endPoints[i].transformID) };
			SolverBody& solverBody{ _solverBodyBuffer->Edit(pointIndex) };
			Vector3 point{ solverBody.position + solverBody.rotation.Rotate(pointConstraint.endPoints[i].localPosition) };

			Vector3 rA{ basePoint - solverBodyBase.position };
			Vector3	rB{ point - solverBody.position };

			Build(
				_constraintBuffer, pointConstraint.tuning,
				pointConstraint.endPoints[0].weight, pointConstraint.endPoints[i].weight,
				3, _batchCount);

			const ConstraintRowBatch& batch(_constraintBuffer->GetBatch(_batchCount));

			std::span<Constraint, 3> constraints{ _constraintBuffer->GetConstraints(batch.firstRow, batch.rowCount).first<3>() };

			ConstraintFunction::CalcPointJacobianAndError(
				basePoint, rA,
				point, rB,
				constraints);

			for (Constraint& constraint: constraints)
			{
				SolveRow(solverBodyBase, solverBody, constraint);
			}

			_batchCount++;
		}
	}
}
// 距離拘束の解く用の拘束のヤコビアンと違反値の再計算
void ConstraintSolverSystem::SolveDistanceConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer, size_t& _batchCount)
{
	// 拘束が存在するかチェック
	if (_constraintStorage->CountDistanceConstraint() <= 0)
	{
		return;
	}
	for (auto id : _constraintStorage->GetDistanceConstraintIDRange())
	{
		const DistanceConstraint& distanceConstraint{ _constraintStorage->GetDistanceConstraint(id) };
		// ポイントが2つ以上じゃないと拘束なんて発生しない
		if (distanceConstraint.endPoints.size() <= 0)
		{
			continue;
		}

		// 基準点となるボディから位置を持ってくる。
		uint32_t basePointIndex{ _solverBodyBuffer->GetIndex(distanceConstraint.ownerEndPoint.transformID) };
		SolverBody& solverBodyBase{ _solverBodyBuffer->Edit(basePointIndex) };
		Vector3 basePoint{ solverBodyBase.position + solverBodyBase.rotation.Rotate(distanceConstraint.ownerEndPoint.localPosition) };

		for (int i{ 0 }; i < distanceConstraint.endPoints.size(); i++)
		{
			// 対象の位置を取得
			uint32_t pointIndex{ _solverBodyBuffer->GetIndex(distanceConstraint.endPoints[i].transformID) };
			SolverBody& solverBody{ _solverBodyBuffer->Edit(pointIndex) };
			Vector3 point{ solverBody.position + solverBody.rotation.Rotate(distanceConstraint.endPoints[i].localPosition) };

			Vector3 rA{ basePoint - solverBodyBase.position };
			Vector3	rB{ point - solverBody.position };

			Build(
				_constraintBuffer, distanceConstraint.tuning,
				distanceConstraint.endPoints[0].weight, distanceConstraint.endPoints[i].weight,
				1, _batchCount);

			const ConstraintRowBatch& batch(_constraintBuffer->GetBatch(_batchCount));

			Constraint& constraint{ _constraintBuffer->Edit(static_cast<uint32_t>(batch.firstRow)) };

			ConstraintFunction::CalcDistanceJacobianAndError(
				distanceConstraint.distance,
				basePoint, rA,
				point, rB,
				constraint
			);

			SolveRow(solverBodyBase, solverBody, constraint);

			_batchCount++;
		}
	}
}
// ヒンジ拘束の解く用の拘束のヤコビアンと違反値の再計算
void ConstraintSolverSystem::SolveHingeConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer, size_t& _batchCount)
{
	// 拘束が存在するかチェック
	if (_constraintStorage->CountHingeConstraint() <= 0)
	{
		return;
	}

	for (auto id : _constraintStorage->GetHingeConstraintIDRange())
	{
		const HingeConstraint& hingeConstraint{ _constraintStorage->GetHingeConstraint(id) };
		// 対象がいないとダメ
		if (hingeConstraint.endPoints.size() < 1)
		{
			continue;
		}

		// 基準点となるボディから位置を持ってくる。
		uint32_t basePointIndex{ _solverBodyBuffer->GetIndex(hingeConstraint.ownerEndPoint.transformID) };
		SolverBody& solverBodyBase{ _solverBodyBuffer->Edit(basePointIndex) };
		Vector3 basePoint{ solverBodyBase.position + solverBodyBase.rotation.Rotate(hingeConstraint.ownerEndPoint.localPosition) };

		Vector3 axis{ hingeConstraint.ownerEndPoint.localRotation.Rotate(Vector3::UP) };

		axis = solverBodyBase.rotation.Rotate(axis).Normalized();

		for (size_t i{ 0 }; i < hingeConstraint.endPoints.size(); i++)
		{
			const EndPointFrame& directionEndPoint{ hingeConstraint.endPoints[i] };
			// 対象の位置を取得
			uint32_t pointIndex{ _solverBodyBuffer->GetIndex(directionEndPoint.transformID) };
			SolverBody& solverBody{ _solverBodyBuffer->Edit(pointIndex) };
			Vector3 point{ solverBody.position + solverBody.rotation.Rotate(directionEndPoint.localPosition) };

			Vector3 rA{ basePoint - solverBodyBase.position };
			Vector3 rB{ point - solverBody.position };

			// B側ヒンジ軸
			Vector3 axisB{ directionEndPoint.localRotation.Rotate(Vector3::UP) };
			axisB = solverBody.rotation.Rotate(axisB);

			// 不正なヒンジ軸
			if (axisB.LengthSqr() <= MathConstants::EPSILON)
			{
				continue;
			}

			axisB.Normalize();

			const ConstraintRowBatch& batch(_constraintBuffer->GetBatch(_batchCount));

			Build(
				_constraintBuffer,
				hingeConstraint.positionTuning, 3,
				hingeConstraint.angularTuning, 2,
				hingeConstraint.ownerEndPoint.weight, directionEndPoint.weight,
				_batchCount);

			std::span<Constraint, 5> constraints{ _constraintBuffer->GetConstraints(batch.firstRow, batch.rowCount).first<5>() };

			ConstraintFunction::CalcHingeJacobianAndError(
				basePoint, rA, axis,
				point, rB, axisB,
				constraints);

			for (Constraint& constraint : constraints)
			{
				SolveRow(solverBodyBase, solverBody, constraint);
			}

			_batchCount++;
		}
	}
}
// 角度制限付き点拘束の解く用の拘束のヤコビアンと違反値の再計算
void ConstraintSolverSystem::SolveAngleLimitPointConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer, size_t& _batchCount)
{
	// 拘束が存在するかチェック
	if (_constraintStorage->CountAngleLimitPointConstraint() <= 0)
	{
		return;
	}

	for (auto id : _constraintStorage->GetAngleLimitPointConstraintIDRange())
	{
		const AngleLimitPointConstraint& angleLimitPointConstraint{ _constraintStorage->GetAngleLimitPointConstraint(id) };
		// ポイントが2つ以上じゃないと拘束なんて発生しない
		if (angleLimitPointConstraint.endPoints.size() < 1)
		{
			continue;
		}

		// 基準点となるボディから位置を持ってくる。
		uint32_t basePointIndex{ _solverBodyBuffer->GetIndex(angleLimitPointConstraint.ownerEndPoint.transformID) };
		SolverBody& solverBodyBase{ _solverBodyBuffer->Edit(basePointIndex) };
		Vector3 basePoint{ solverBodyBase.position + solverBodyBase.rotation.Rotate(angleLimitPointConstraint.ownerEndPoint.localPosition) };
		Vector3 baseDir{ angleLimitPointConstraint.ownerEndPoint.localRotation.Rotate(Vector3::UP) };
		baseDir = solverBodyBase.rotation.Rotate(baseDir);

		for (size_t i{ 0 }; i < angleLimitPointConstraint.endPoints.size(); i++)
		{
			const EndPointFrame& endPoint{ angleLimitPointConstraint.endPoints[i] };
			// 対象の位置を取得
			uint32_t pointIndex{ _solverBodyBuffer->GetIndex(endPoint.transformID) };
			SolverBody& solverBody{ _solverBodyBuffer->Edit(pointIndex) };
			Vector3 point{ solverBody.position + solverBody.rotation.Rotate(endPoint.localPosition) };
			Vector3 dir{ endPoint.localRotation.Rotate(Vector3::RIGHT) };
			dir = solverBody.rotation.Rotate(dir);

			Vector3 rA = basePoint - solverBodyBase.position;
			Vector3	rB = point - solverBody.position;

			const ConstraintRowBatch& batch(_constraintBuffer->GetBatch(_batchCount));

			Build(
				_constraintBuffer, angleLimitPointConstraint.tuning,
				angleLimitPointConstraint.ownerEndPoint.weight, endPoint.weight,
				4, _batchCount);

			std::span<Constraint, 4> constraints{ _constraintBuffer->GetConstraints(batch.firstRow, batch.rowCount).first<4>() };

			ConstraintFunction::CalcAngleLimitPointJacobianAndError(
				angleLimitPointConstraint.angleMax, angleLimitPointConstraint.angleMin,
				basePoint, rA, baseDir,
				point, rB, dir,
				constraints);

			for (Constraint& constraint : constraints)
			{
				SolveRow(solverBodyBase, solverBody, constraint);
			}

			_batchCount++;
		}
	}
}
// 角度制限付きヒンジ拘束の解く用の拘束のヤコビアンと違反値の再計算
void ConstraintSolverSystem::SolveAngleLimitHingeConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer, size_t& _batchCount)
{
	// 拘束が存在するかチェック
	if (_constraintStorage->CountAngleLimitHingeConstraint() <= 0)
	{
		return;
	}

	for (auto id : _constraintStorage->GetAngleLimitHingeConstraintIDRange())
	{
		const AngleLimitHingeConstraint& angleLimitHingeConstraint{ _constraintStorage->GetAngleLimitHingeConstraint(id) };
		// 対象がいないとダメ
		if (angleLimitHingeConstraint.endPoints.size() < 1)
		{
			continue;
		}

		// 基準側
		const EndPointFrame& ownerEndPoint{ angleLimitHingeConstraint.ownerEndPoint };
		uint32_t basePointIndex{ _solverBodyBuffer->GetIndex(ownerEndPoint.transformID) };
		SolverBody& solverBodyBase{ _solverBodyBuffer->Edit(basePointIndex) };
		Vector3 basePoint{ solverBodyBase.position + solverBodyBase.rotation.Rotate(ownerEndPoint.localPosition) };
		Vector3 axis{ solverBodyBase.rotation.Rotate(ownerEndPoint.localRotation.Rotate(Vector3::UP)) };

		// A側の角度0基準方向
		Vector3 referenceA{ solverBodyBase.rotation.Rotate(ownerEndPoint.localRotation.Rotate(Vector3::RIGHT)) };

		for (size_t i{ 0 }; i < angleLimitHingeConstraint.endPoints.size(); i++)
		{
			const EndPointFrame& endPoint{ angleLimitHingeConstraint.endPoints[i] };
			// 対象側
			uint32_t pointIndex{ _solverBodyBuffer->GetIndex(endPoint.transformID) };
			SolverBody& solverBody{ _solverBodyBuffer->Edit(pointIndex) };
			Vector3 point{ solverBody.position + solverBody.rotation.Rotate(endPoint.localPosition) };
			Vector3 axisB{ solverBody.rotation.Rotate(endPoint.localRotation.Rotate(Vector3::UP)) };
			// B側の基準方向をA側ヒンジ軸の平面へ射影
			Vector3 referenceB{ solverBody.rotation.Rotate(endPoint.localRotation.Rotate(Vector3::RIGHT)) };

			if (axisB.LengthSqr() <= MathConstants::EPSILON)
			{
				continue;
			}

			axisB.Normalize();

			Vector3 rA{ basePoint - solverBodyBase.position };
			Vector3 rB{ point - solverBody.position };

			Build(
				_constraintBuffer,
				angleLimitHingeConstraint.positionTuning, 3,
				angleLimitHingeConstraint.angularTuning, 3,
				ownerEndPoint.weight, endPoint.weight,
				_batchCount);

			const ConstraintRowBatch& batch(_constraintBuffer->GetBatch(_batchCount));

			std::span<Constraint, 6> constraints{ _constraintBuffer->GetConstraints(batch.firstRow, batch.rowCount).first<6>() };

			ConstraintFunction::CalcAngleLimitHingeJacobianAndError(
				angleLimitHingeConstraint.angleMax, angleLimitHingeConstraint.angleMin,
				basePoint, rA, axis, referenceA,
				point, rB, axisB, referenceB,
				constraints);

			for (Constraint& constraint : constraints)
			{
				SolveRow(solverBodyBase, solverBody, constraint);
			}

			_batchCount++;
		}
	}
}
// SwingTwist拘束の解く用の拘束のヤコビアンと違反値の再計算
void ConstraintSolverSystem::SolveLimitedBallJointConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer, size_t& _batchCount)
{
	// 拘束が存在するかチェック
	if (_constraintStorage->CountLimitedBallJointConstraint() <= 0)
	{
		return;
	}

	for (auto id : _constraintStorage->GetLimitedBallJointConstraintIDRange())
	{
		const LimitedBallJointConstraint& limitedBallJointConstraint{ _constraintStorage->GetLimitedBallJointConstraint(id) };
		// ポイントが2つ以上じゃないと拘束なんて発生しない
		if (limitedBallJointConstraint.endPoints.size() < 1)
		{
			continue;
		}

		const EndPointFrame& ownerEndPoint{ limitedBallJointConstraint.ownerEndPoint };

		// 基準点となるボディから位置を持ってくる。
		uint32_t basePointIndex{ _solverBodyBuffer->GetIndex(ownerEndPoint.transformID) };
		SolverBody& solverBodyBase{ _solverBodyBuffer->Edit(basePointIndex) };
		Vector3 basePoint{ solverBodyBase.position + solverBodyBase.rotation.Rotate(ownerEndPoint.localPosition) };
		Vector3 baseAxis{ ownerEndPoint.localRotation.Rotate(Vector3::UP) };
		baseAxis = solverBodyBase.rotation.Rotate(baseAxis);

		// A側の角度0基準方向
		Vector3 referenceA{ solverBodyBase.rotation.Rotate(ownerEndPoint.localRotation.Rotate(Vector3::RIGHT)) };

		for (size_t i{ 0 }; i < limitedBallJointConstraint.endPoints.size(); i++)
		{
			const EndPointFrame& endPoint{ limitedBallJointConstraint.endPoints[i] };
			// 対象の位置を取得
			uint32_t pointIndex{ _solverBodyBuffer->GetIndex(endPoint.transformID) };
			SolverBody& solverBody{ _solverBodyBuffer->Edit(pointIndex) };
			Vector3 point{ solverBody.position + solverBody.rotation.Rotate(endPoint.localPosition) };
			Vector3 axis{ endPoint.localRotation.Rotate(Vector3::UP) };
			axis = solverBody.rotation.Rotate(axis);
			// B側の基準方向
			Vector3 referenceB{ solverBody.rotation.Rotate(endPoint.localRotation.Rotate(Vector3::RIGHT)) };

			Vector3 rA{ basePoint - solverBodyBase.position };
			Vector3	rB{ point - solverBody.position };

			Build(_constraintBuffer,
				limitedBallJointConstraint.positionTuning, 3,
				limitedBallJointConstraint.angularTuning, 2,
				ownerEndPoint.weight, endPoint.weight,
				_batchCount);

			const ConstraintRowBatch& batch(_constraintBuffer->GetBatch(_batchCount));

			std::span<Constraint, 5> constraints{ _constraintBuffer->GetConstraints(batch.firstRow, batch.rowCount).first<5>() };

			ConstraintFunction::CalcLimitedBallJointJacobianAndError(
				limitedBallJointConstraint.swingAngle, limitedBallJointConstraint.twistAngleMax, limitedBallJointConstraint.twistAngleMin,
				basePoint, rA, baseAxis, referenceA,
				point, rB, axis, referenceB,
				constraints);

			for (Constraint& constraint : constraints)
			{
				SolveRow(solverBodyBase, solverBody, constraint);
			}

			_batchCount++;
		}
	}
}
// 関節駆動拘束の解く用の拘束のヤコビアンと違反値の再計算
void ConstraintSolverSystem::SolveJointDriveConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer, size_t& _batchCount)
{
	// 拘束が存在するかチェック
	if (_constraintStorage->CountJointDriveConstraint() <= 0)
	{
		return;
	}

	for (auto id : _constraintStorage->GetJointDriveConstraintIDRange())
	{
		const JointDriveConstraint& jointDrive{ _constraintStorage->GetJointDriveConstraint(id) };
		// 相手ポイントが無効値なら飛ばす
		if (!jointDrive.otherEndPoint.transformID.IsValid())
		{
			continue;
		}

		// 基準点となるボディから位置を持ってくる。
		const EndPointFrame& ownerEndPoint{ jointDrive.ownerEndPoint };

		uint32_t basePointIndex{ _solverBodyBuffer->GetIndex(ownerEndPoint.transformID) };
		SolverBody& solverBodyBase{ _solverBodyBuffer->Edit(basePointIndex) };
		const Quaternion& baseRot{ solverBodyBase.rotation * ownerEndPoint.localRotation };

		// 相手となるボディから位置を持ってくる。
		const EndPointFrame& endPoint{ jointDrive.otherEndPoint };

		uint32_t otherPointIndex{ _solverBodyBuffer->GetIndex(endPoint.transformID) };
		SolverBody& solverBodyOther{ _solverBodyBuffer->Edit(otherPointIndex) };
		const Quaternion& otherRot{ solverBodyOther.rotation * endPoint.localRotation };

		Build(
			_constraintBuffer, jointDrive.tuning,
			ownerEndPoint.weight, endPoint.weight,
			3, _batchCount);

		const ConstraintRowBatch& batch(_constraintBuffer->GetBatch(_batchCount));

		std::span<Constraint,3> constraints{ _constraintBuffer->GetConstraints(batch.firstRow, batch.rowCount).first<3>() };

		ConstraintFunction::CalcJointDriveJacobianAndError(
			jointDrive.targetRelativeRotation,
			baseRot,
			otherRot,
			_constraintBuffer->GetConstraints(batch.firstRow, batch.rowCount).first<3>());

		for (Constraint& constraint : constraints)
		{
			SolveRow(solverBodyBase, solverBodyOther, constraint);
		}

		_batchCount++;
	}
}

// ある点を線上の動きに制限する拘束のヤコビアンと違反値の計算
void ConstraintSolverSystem::SolvePointOnLineConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer, size_t& _batchCount)
{

}

// スライダー拘束のヤコビアンと違反値の計算
void ConstraintSolverSystem::SolveSliderConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer, size_t& _batchCount)
{

}

// 距離制限のある、ある点を線上の動きに制限する拘束のヤコビアンと違反値の計算
void ConstraintSolverSystem::SolveLimitedPointOnLineConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer, size_t& _batchCount)
{

}

// 距離制限のある、スライダー拘束のヤコビアンと違反値の計算
void ConstraintSolverSystem::SolveLimitedSliderConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer, size_t& _batchCount)
{

}