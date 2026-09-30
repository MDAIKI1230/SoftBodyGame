#include <algorithm>

#include "ConstraintFunction.h"

#include "TimeManager.h"

#include "ConstraintBuildSystem.h"

// 最初の拘束生成
void ConstraintBuildSystem::Build(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer)
{
	BuildPointConstraint(_constraintStorage, _solverBodyBuffer, _constraintBuffer);
	BuildDistanceConstraint(_constraintStorage, _solverBodyBuffer, _constraintBuffer);
	BuildHingeConstraint(_constraintStorage, _solverBodyBuffer, _constraintBuffer);
	BuildAngleLimitPointConstraint(_constraintStorage, _solverBodyBuffer, _constraintBuffer);
	BuildAngleLimitHingeConstraint(_constraintStorage, _solverBodyBuffer, _constraintBuffer);
	BuildLimitedBallJointConstraint(_constraintStorage, _solverBodyBuffer, _constraintBuffer);
	BuildJointDriveConstraint(_constraintStorage, _solverBodyBuffer, _constraintBuffer);
}

// 最初の衝突の拘束生成
void ConstraintBuildSystem::Build(ColliderStorage* _colliderStorage, SolverBodyBuffer* _solverBodyBuffer, CollisionManifoldBuffer* _manifoldBuffer, ConstraintBuffer* _constraintBuffer)
{
	// すべての衝突情報から拘束条件の作成をする
	for (auto& manifold : _manifoldBuffer->GetAll())
	{
		for (int i{ 0 }; i < manifold.pointCount; i++)
		{
			Constraint constraint;
			// SolverBodyのIndexを取得
			PhysicsTransformID transformID{ _colliderStorage->GetTransformID(manifold.colliderA) };
			constraint.solverBodyAIndex = _solverBodyBuffer->GetIndex(transformID);
			transformID = _colliderStorage->GetTransformID(manifold.colliderB);
			constraint.solverBodyBIndex = _solverBodyBuffer->GetIndex(transformID);

			Vector3 positionLocalA{ manifold.points[i].positionLocalA };
			Vector3 positionLocalB{ manifold.points[i].positionLocalB };

			Vector3 normal{ manifold.normal };

			// ボディA
			SolverBody& solverBodyA{ _solverBodyBuffer->Edit(constraint.solverBodyAIndex) };
			// ボディB
			SolverBody& solverBodyB{ _solverBodyBuffer->Edit(constraint.solverBodyBIndex) };

			// 重心から衝突点ベクトル
			Vector3 rA{ solverBodyA.rotation.Rotate(positionLocalA) };
			Vector3 rB{ solverBodyB.rotation.Rotate(positionLocalB) };

			// 重心から衝突点ベクトルAと法線の外積
			Vector3 rACross{ Vector3::Cross(rA,normal) };
			// 重心から衝突点ベクトルBと法線の外積
			Vector3 rBCross{ Vector3::Cross(rB,normal) };

			Vector3 pointA{ solverBodyA.position + rA };
			Vector3 pointB{ solverBodyB.position + rB };

			float sep{ Vector3::Dot((pointB - pointA), normal) };

			constraint.error = std::max(-sep, 0.0f);
			
			constraint.jacobian[0] = normal;
			constraint.jacobian[1] = rACross;
			constraint.jacobian[2] = normal;
			constraint.jacobian[3] = rBCross;

			constraint.minLambda = 0.0f;

			_constraintBuffer->Add(constraint);
		}
	}
}

// エラー等の変化値の再計算
void ConstraintBuildSystem::RefreshRows(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer)
{
	// 点拘束の解く用の拘束のヤコビアンと違反値の再計算
	RefreshPointConstraint(_constraintStorage, _solverBodyBuffer, _constraintBuffer);
	// 距離拘束の解く用の拘束のヤコビアンと違反値の再計算
	RefreshDistanceConstraint(_constraintStorage, _solverBodyBuffer, _constraintBuffer);
	// ヒンジ拘束の解く用の拘束のヤコビアンと違反値の再計算
	RefreshHingeConstraint(_constraintStorage, _solverBodyBuffer, _constraintBuffer);
	// 角度制限付き点拘束の解く用の拘束のヤコビアンと違反値の再計算
	RefreshAngleLimitPointConstraint(_constraintStorage, _solverBodyBuffer, _constraintBuffer);
	// 角度制限付きヒンジ拘束の解く用の拘束のヤコビアンと違反値の再計算
	RefreshAngleLimitHingeConstraint(_constraintStorage, _solverBodyBuffer, _constraintBuffer);
	// SwingTwist拘束の解く用の拘束のヤコビアンと違反値の再計算
	RefreshLimitedBallJointConstraint(_constraintStorage, _solverBodyBuffer, _constraintBuffer);
	// 関節駆動拘束の解く用の拘束のヤコビアンと違反値の再計算
	RefreshJointDriveConstraint(_constraintStorage, _solverBodyBuffer, _constraintBuffer);
}

// 点拘束の解く用の拘束構造体を作る
void ConstraintBuildSystem::BuildPointConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer)
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
		if (pointConstraint.endPoints.size() <= 1)
		{
			continue;
		}

		// 基準点となるボディから位置を持ってくる。
		uint32_t basePointIndex{ _solverBodyBuffer->GetIndex(pointConstraint.endPoints[0].transformID) };
		const SolverBody& solverBodyBase{ _solverBodyBuffer->Get(basePointIndex) };
		Vector3 basePoint{ solverBodyBase.position + solverBodyBase.rotation.Rotate(pointConstraint.endPoints[0].localPosition) };

		for (int i{ 1 }; i < pointConstraint.endPoints.size(); i++)
		{
			Constraint constraint;

			// 対象の位置を取得
			uint32_t pointIndex{ _solverBodyBuffer->GetIndex(pointConstraint.endPoints[i].transformID) };
			const SolverBody& solverBody{ _solverBodyBuffer->Get(pointIndex) };
			Vector3 point{ solverBody.position + solverBody.rotation.Rotate(pointConstraint.endPoints[i].localPosition) };

			constraint.solverBodyAIndex = basePointIndex;
			constraint.solverBodyBIndex = pointIndex;

			Vector3 rA = basePoint - solverBodyBase.position;
			Vector3	rB = point - solverBody.position;

			ConstraintRowBatch batch;
			batch.sourceConstraintID = id;
			batch.endpointIndex = i;
			batch.firstRow = _constraintBuffer->GetSize();
			batch.rowCount = 3;

			_constraintBuffer->AddBatch(batch);

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
	for (auto id : _constraintStorage->GetDistanceConstraintIDRange())
	{
		const DistanceConstraint& distanceConstraint{ _constraintStorage->GetDistanceConstraint(id) };
		// ポイントが2つ以上じゃないと拘束なんて発生しない
		if (distanceConstraint.endPoints.size() <= 1)
		{
			continue;
		}

		// 基準点となるボディから位置を持ってくる。
		uint32_t basePointIndex{ _solverBodyBuffer->GetIndex(distanceConstraint.endPoints[0].transformID) };
		const SolverBody& solverBodyBase{ _solverBodyBuffer->Get(basePointIndex) };
		Vector3 basePoint{ solverBodyBase.position + solverBodyBase.rotation.Rotate(distanceConstraint.endPoints[0].localPosition) };

		for (int i{ 1 }; i < distanceConstraint.endPoints.size(); i++)
		{
			Constraint constraint;

			// 対象の位置を取得
			uint32_t pointIndex{ _solverBodyBuffer->GetIndex(distanceConstraint.endPoints[i].transformID) };
			const SolverBody& solverBody{ _solverBodyBuffer->Get(pointIndex) };
			Vector3 point{ solverBody.position + solverBody.rotation.Rotate(distanceConstraint.endPoints[i].localPosition) };

			constraint.solverBodyAIndex = basePointIndex;
			constraint.solverBodyBIndex = pointIndex;

			Vector3 rA{ basePoint - solverBodyBase.position };
			Vector3	rB{ point - solverBody.position };

			ConstraintRowBatch batch;
			batch.sourceConstraintID = id;
			batch.endpointIndex = i;
			batch.firstRow = _constraintBuffer->GetSize();
			batch.rowCount = 1;

			_constraintBuffer->AddBatch(batch);

			ConstraintFunction::CalcDistanceJacobianAndError(
				distanceConstraint.distance,
				basePoint, rA,
				point, rB,
				constraint
			);

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
		const SolverBody& solverBodyBase{ _solverBodyBuffer->Get(basePointIndex) };
		Vector3 basePoint{ solverBodyBase.position + solverBodyBase.rotation.Rotate(hingeConstraint.ownerEndPoint.localPosition) };

		Vector3 axis{ hingeConstraint.ownerEndPoint.localRotation.Rotate(Vector3::UP) };

		axis = solverBodyBase.rotation.Rotate(axis).Normalized();

		for (size_t i{ 0 }; i < hingeConstraint.endPoints.size(); i++)
		{
			const EndPointFrame& directionEndPoint{ hingeConstraint.endPoints[i] };
			// 対象の位置を取得
			uint32_t pointIndex{ _solverBodyBuffer->GetIndex(directionEndPoint.transformID) };
			const SolverBody& solverBody{ _solverBodyBuffer->Get(pointIndex) };
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

			Constraint constraints[5];

			ConstraintRowBatch batch;
			batch.sourceConstraintID = id;
			batch.endpointIndex = i;
			batch.firstRow = _constraintBuffer->GetSize();
			batch.rowCount = 5;

			_constraintBuffer->AddBatch(batch);

			ConstraintFunction::CalcHingeJacobianAndError(
				basePoint, rA, axis,
				point, rB, axisB,
				constraints);

			// 点拘束の情報づくり
			for (size_t i{ 0 }; i < 3; i++)
			{
				// インデックスを詰める
				constraints[i].solverBodyAIndex = basePointIndex;
				constraints[i].solverBodyBIndex = pointIndex;

				// 情報詰め
				MakeConstraintInfo(constraints[i], hingeConstraint.positionTuning);

				_constraintBuffer->Add(constraints[i]);
			}

			// 軸拘束の情報づくり
			for (size_t i{ 3 }; i < 5; i++)
			{
				// インデックスを詰める
				constraints[i].solverBodyAIndex = basePointIndex;
				constraints[i].solverBodyBIndex = pointIndex;

				// 情報詰め
				MakeConstraintInfo(constraints[i], hingeConstraint.angularTuning);

				_constraintBuffer->Add(constraints[i]);
			}
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
		const SolverBody& solverBodyBase{ _solverBodyBuffer->Get(basePointIndex) };
		Vector3 basePoint{ solverBodyBase.position + solverBodyBase.rotation.Rotate(angleLimitPointConstraint.ownerEndPoint.localPosition) };
		Vector3 baseDir{ angleLimitPointConstraint.ownerEndPoint.localRotation.Rotate(Vector3::UP) };
		baseDir = solverBodyBase.rotation.Rotate(baseDir);

		for (size_t i{ 0 }; i < angleLimitPointConstraint.endPoints.size(); i++)
		{
			const EndPointFrame& endPoint{ angleLimitPointConstraint.endPoints[i] };
			// 対象の位置を取得
			uint32_t pointIndex{ _solverBodyBuffer->GetIndex(endPoint.transformID) };
			const SolverBody& solverBody{ _solverBodyBuffer->Get(pointIndex) };
			Vector3 point{ solverBody.position + solverBody.rotation.Rotate(endPoint.localPosition) };
			Vector3 dir{ endPoint.localRotation.Rotate(Vector3::RIGHT) };
			dir = solverBody.rotation.Rotate(dir);

			Vector3 rA = basePoint - solverBodyBase.position;
			Vector3	rB = point - solverBody.position;

			Constraint constraints[4];

			ConstraintFunction::CalcAngleLimitPointJacobianAndError(
				angleLimitPointConstraint.angleMax, angleLimitPointConstraint.angleMin,
				basePoint, rA, baseDir,
				point, rB, dir,
				constraints);

			ConstraintRowBatch batch;
			batch.sourceConstraintID = id;
			batch.endpointIndex = i;
			batch.firstRow = _constraintBuffer->GetSize();
			batch.rowCount = 4;

			_constraintBuffer->AddBatch(batch);

			for (size_t i{ 0 }; i < 4; i++)
			{
				constraints[i].solverBodyAIndex = basePointIndex;
				constraints[i].solverBodyBIndex = pointIndex;

				MakeConstraintInfo(constraints[i], angleLimitPointConstraint.tuning);

				_constraintBuffer->Add(constraints[i]);
			}
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

	for (auto id  : _constraintStorage->GetAngleLimitHingeConstraintIDRange())
	{
		const AngleLimitHingeConstraint& angleLimitHingeConstraint{ _constraintStorage->GetAngleLimitHingeConstraint(id) };
		// 対象がいないとダメ
		if (angleLimitHingeConstraint.endPoints.size() < 1)
		{
			continue;
		}

		// 基準側
		const EndPointFrame& endPoint{ angleLimitHingeConstraint.ownerEndPoint };
		uint32_t basePointIndex{ _solverBodyBuffer->GetIndex(endPoint.transformID) };
		const SolverBody& solverBodyBase{ _solverBodyBuffer->Get(basePointIndex) };
		Vector3 basePoint{ solverBodyBase.position + solverBodyBase.rotation.Rotate(endPoint.localPosition) };
		Vector3 axis{ solverBodyBase.rotation.Rotate(endPoint.localRotation.Rotate(Vector3::UP)) };

		// A側の角度0基準方向
		Vector3 referenceA{ solverBodyBase.rotation.Rotate(endPoint.localRotation.Rotate(Vector3::RIGHT)) };

		for (size_t i{ 0 }; i < angleLimitHingeConstraint.endPoints.size(); i++)
		{
			const EndPointFrame endPoint{ angleLimitHingeConstraint.endPoints[i] };
			// 対象側
			uint32_t pointIndex{ _solverBodyBuffer->GetIndex(endPoint.transformID) };
			const SolverBody& solverBody{ _solverBodyBuffer->Get(pointIndex) };
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

			Constraint constraints[6];

			ConstraintFunction::CalcAngleLimitHingeJacobianAndError(
				angleLimitHingeConstraint.angleMax, angleLimitHingeConstraint.angleMin,
				basePoint, rA, axis, referenceA,
				point, rB, axisB, referenceB,
				constraints);

			ConstraintRowBatch batch;
			batch.sourceConstraintID = id;
			batch.endpointIndex = i;
			batch.firstRow = _constraintBuffer->GetSize();
			batch.rowCount = 6;

			_constraintBuffer->AddBatch(batch);

			// 最初の三つは点拘束
			for (size_t i{ 0 }; i < 3; i++)
			{
				constraints[i].solverBodyAIndex = basePointIndex;
				constraints[i].solverBodyBIndex = pointIndex;

				MakeConstraintInfo(constraints[i], angleLimitHingeConstraint.positionTuning);

				_constraintBuffer->Add(constraints[i]);
			}
			// 残りは角度
			for (size_t i{ 3 }; i < 6; i++)
			{
				constraints[i].solverBodyAIndex = basePointIndex;
				constraints[i].solverBodyBIndex = pointIndex;

				MakeConstraintInfo(constraints[i], angleLimitHingeConstraint.angularTuning);

				_constraintBuffer->Add(constraints[i]);
			}
		}
	}
}

// SwingTwist拘束の解く用の拘束構造体を作る
void ConstraintBuildSystem::BuildLimitedBallJointConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer)
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
		const SolverBody& solverBodyBase{ _solverBodyBuffer->Get(basePointIndex) };
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
			const SolverBody& solverBody{ _solverBodyBuffer->Get(pointIndex) };
			Vector3 point{ solverBody.position + solverBody.rotation.Rotate(endPoint.localPosition) };
			Vector3 axis{ endPoint.localRotation.Rotate(Vector3::UP) };
			axis = solverBody.rotation.Rotate(axis);
			// B側の基準方向
			Vector3 referenceB{ solverBody.rotation.Rotate(endPoint.localRotation.Rotate(Vector3::RIGHT)) };

			Vector3 rA{ basePoint - solverBodyBase.position };
			Vector3	rB{ point - solverBody.position };

			Constraint constraints[5];

			ConstraintFunction::CalcLimitedBallJointJacobianAndError(
				limitedBallJointConstraint.swingAngle, limitedBallJointConstraint.twistAngleMax, limitedBallJointConstraint.twistAngleMin,
				basePoint, rA, baseAxis, referenceA,
				point, rB, axis, referenceB,
				constraints);

			ConstraintRowBatch batch;
			batch.sourceConstraintID = id;
			batch.endpointIndex = i;
			batch.firstRow = _constraintBuffer->GetSize();
			batch.rowCount = 5;

			_constraintBuffer->AddBatch(batch);

			// 最初の三つは点拘束
			for (size_t i{ 0 }; i < 3; i++)
			{
				constraints[i].solverBodyAIndex = basePointIndex;
				constraints[i].solverBodyBIndex = pointIndex;

				MakeConstraintInfo(constraints[i], limitedBallJointConstraint.positionTuning);

				_constraintBuffer->Add(constraints[i]);
			}
			// 残りの角度をやる
			for (size_t i{ 3 }; i < 5; i++)
			{
				constraints[i].solverBodyAIndex = basePointIndex;
				constraints[i].solverBodyBIndex = pointIndex;

				MakeConstraintInfo(constraints[i], limitedBallJointConstraint.angularTuning);

				_constraintBuffer->Add(constraints[i]);
			}
		}
	}
}

// 関節駆動拘束の解く用の拘束構造体を作る
void ConstraintBuildSystem::BuildJointDriveConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer)
{
	// 拘束が存在するかチェック
	if (_constraintStorage->CountJointDriveConstraint() <= 0)
	{
		return;
	}

	for (auto id : _constraintStorage->GetJointDriveConstraintConstraintIDRange())
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
		const SolverBody& solverBodyBase{ _solverBodyBuffer->Get(basePointIndex) };
		const Quaternion& baseRot{ solverBodyBase.rotation * ownerEndPoint.localRotation };

		// 相手となるボディから位置を持ってくる。
		const EndPointFrame& otherEndPoint{ jointDrive.otherEndPoint };

		uint32_t otherPointIndex{ _solverBodyBuffer->GetIndex(otherEndPoint.transformID) };
		const SolverBody& solverBodyOther{ _solverBodyBuffer->Get(otherPointIndex) };
		const Quaternion& otherRot{ solverBodyOther.rotation * otherEndPoint.localRotation };

		Constraint constraints[3];

		ConstraintFunction::CalcJointDriveJacobianAndError(
			jointDrive.targetRelativeRotation,
			baseRot,
			otherRot,
			constraints);

		ConstraintRowBatch batch;
		batch.sourceConstraintID = id;
		batch.endpointIndex = 0;
		batch.firstRow = _constraintBuffer->GetSize();
		batch.rowCount = 3;

		_constraintBuffer->AddBatch(batch);

		for (size_t i{ 0 }; i < 3; i++)
		{
			constraints[i].solverBodyAIndex = basePointIndex;
			constraints[i].solverBodyBIndex = otherPointIndex;

			MakeConstraintInfo(constraints[i], jointDrive.tuning);

			_constraintBuffer->Add(constraints[i]);
		}
	}
}

void ConstraintBuildSystem::MakeConstraintInfo(Constraint& _constraint, const ConstraintTuning& _tuning)
{
	float deltaTime{ TimeManager::GetFixedDeltaTime() };

	_constraint.timeStepAdjustedCompliance = _tuning.compliance / (deltaTime * deltaTime);

	_constraint.minLambda = -_tuning.maxForce * deltaTime;
	_constraint.maxLambda = _tuning.maxForce * deltaTime;
}

// 点拘束の解く用の拘束のヤコビアンと違反値の再計算
void ConstraintBuildSystem::RefreshPointConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer)
{
	// 拘束が存在するかチェック
	if (_constraintStorage->CountPointConstraint() <= 0)
	{
		return;
	}

	uint32_t batchCount{ 0 };
	for (auto id : _constraintStorage->GetPointConstraintIDRange())
	{
		const PointConstraint& pointConstraint{ _constraintStorage->GetPointConstraint(id) };
		// ポイントが2つ以上じゃないと拘束なんて発生しない
		if (pointConstraint.endPoints.size() <= 1)
		{
			continue;
		}

		// 基準点となるボディから位置を持ってくる。
		uint32_t basePointIndex{ _solverBodyBuffer->GetIndex(pointConstraint.endPoints[0].transformID) };
		const SolverBody& solverBodyBase{ _solverBodyBuffer->Get(basePointIndex) };
		Vector3 basePoint{ solverBodyBase.position + solverBodyBase.rotation.Rotate(pointConstraint.endPoints[0].localPosition) };

		for (int i{ 1 }; i < pointConstraint.endPoints.size(); i++)
		{
			Constraint constraint;

			// 対象の位置を取得
			uint32_t pointIndex{ _solverBodyBuffer->GetIndex(pointConstraint.endPoints[i].transformID) };
			const SolverBody& solverBody{ _solverBodyBuffer->Get(pointIndex) };
			Vector3 point{ solverBody.position + solverBody.rotation.Rotate(pointConstraint.endPoints[i].localPosition) };

			constraint.solverBodyAIndex = basePointIndex;
			constraint.solverBodyBIndex = pointIndex;

			Vector3 rA{ basePoint - solverBodyBase.position };
			Vector3	rB{ point - solverBody.position };

			const ConstraintRowBatch& batch(_constraintBuffer->GetBatch(batchCount));

			ConstraintFunction::CalcPointJacobianAndError(
				basePoint, rA,
				point, rB,
				_constraintBuffer->GetConstraints(batch.firstRow, batch.rowCount).first<3>());
		}
	}
}
// 距離拘束の解く用の拘束のヤコビアンと違反値の再計算
void ConstraintBuildSystem::RefreshDistanceConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer)
{

}
// ヒンジ拘束の解く用の拘束のヤコビアンと違反値の再計算
void ConstraintBuildSystem::RefreshHingeConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer)
{

}
// 角度制限付き点拘束の解く用の拘束のヤコビアンと違反値の再計算
void ConstraintBuildSystem::RefreshAngleLimitPointConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer)
{

}
// 角度制限付きヒンジ拘束の解く用の拘束のヤコビアンと違反値の再計算
void ConstraintBuildSystem::RefreshAngleLimitHingeConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer)
{

}
// SwingTwist拘束の解く用の拘束のヤコビアンと違反値の再計算
void ConstraintBuildSystem::RefreshLimitedBallJointConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer)
{

}
// 関節駆動拘束の解く用の拘束のヤコビアンと違反値の再計算
void ConstraintBuildSystem::RefreshJointDriveConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer)
{

}

// 点拘束を情報からソルバ用拘束に変換してバッファに入れる
void ConstraintBuildSystem::AddPointConstraint(
	Constraint _constraint, const ConstraintTuning& _tuning,
	const Vector3& _rA, const Vector3& _pointA,
	const Vector3& _rB, const Vector3& _pointB,
	ConstraintBuffer* _constraintBuffer)
{
	Constraint constraints[3]{ _constraint,_constraint,_constraint };
	ConstraintFunction::CalcPointJacobianAndError(
		_pointA, _rA,
		_pointB, _rB,
		constraints);

	for (Constraint& constraint : constraints)
	{
		MakeConstraintInfo(constraint, _tuning);
		_constraintBuffer->Add(constraint);
	}
}

void ConstraintBuildSystem::AddAxisConstraint(
	Constraint _constraint, const ConstraintTuning& _tuning,
	const Vector3& _tangent, const Vector3& _axisError,
	ConstraintBuffer* _constraintBuffer)
{
	ConstraintFunction::CalcAxisJacobianAndError(_tangent, _axisError, _constraint);

	MakeConstraintInfo(_constraint, _tuning);

	_constraintBuffer->Add(_constraint);
};