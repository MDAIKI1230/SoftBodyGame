#include "ConstraintBuildSystem.h"

void ConstraintBuildSystem::FixedUpdate(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer)
{
	BuildPointConstraint(_constraintStorage, _solverBodyBuffer, _constraintBuffer);
	BuildDistanceConstraint(_constraintStorage, _solverBodyBuffer, _constraintBuffer);
}

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

			// 差
			Vector3 diff{ basePoint - point };
			// 差をそのまま拘束Cの結果とする
			constraint.error = diff.x;


			// ヤコビアンの計算
			constraint.jacobian[0] = Vector3::RIGHT;
			constraint.jacobian[1] = Vector3::Cross(rA,Vector3::RIGHT);
			constraint.jacobian[2] = -Vector3::RIGHT;
			constraint.jacobian[3] = -Vector3::Cross(rB, Vector3::RIGHT);

			// 拘束として追加
			_constraintBuffer->Add(constraint);

			// 差をそのまま拘束Cの結果とする
			constraint.error = diff.y;


			// ヤコビアンの計算
			constraint.jacobian[0] = Vector3::UP;
			constraint.jacobian[1] = Vector3::Cross(rA, Vector3::UP);
			constraint.jacobian[2] = -Vector3::UP;
			constraint.jacobian[3] = -Vector3::Cross(rB, Vector3::UP);

			// 拘束として追加
			_constraintBuffer->Add(constraint);

			// 差をそのまま拘束Cの結果とする
			constraint.error = diff.z;


			// ヤコビアンの計算
			constraint.jacobian[0] = Vector3::FORWARD;
			constraint.jacobian[1] = Vector3::Cross(rA, Vector3::FORWARD);
			constraint.jacobian[2] = -Vector3::FORWARD;
			constraint.jacobian[3] = -Vector3::Cross(rB, Vector3::FORWARD);

			// 拘束として追加
			_constraintBuffer->Add(constraint);
		}
	}
}

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

			// 拘束として追加
			_constraintBuffer->Add(constraint);
		}
	}
}
