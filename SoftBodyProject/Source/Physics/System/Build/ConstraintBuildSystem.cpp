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

			// 差
			Vector3 diff{ basePoint - point };
			// 差をそのまま拘束Cの結果とする
			constraint.error = diff.x;


			// ヤコビアンの計算
			constraint.jacobian[0] = Vector3::RIGHT;
			constraint.jacobian[1] = Vector3::Cross(rA,Vector3::RIGHT);
			constraint.jacobian[2] = -Vector3::RIGHT;
			constraint.jacobian[3] = -Vector3::Cross(rB, Vector3::RIGHT);

			MakeConstraintInfo(constraint, pointConstraint);

			// 拘束として追加
			_constraintBuffer->Add(constraint);

			// 差をそのまま拘束Cの結果とする
			constraint.error = diff.y;


			// ヤコビアンの計算
			constraint.jacobian[0] = Vector3::UP;
			constraint.jacobian[1] = Vector3::Cross(rA, Vector3::UP);
			constraint.jacobian[2] = -Vector3::UP;
			constraint.jacobian[3] = -Vector3::Cross(rB, Vector3::UP);

			MakeConstraintInfo(constraint, pointConstraint);

			// 拘束として追加
			_constraintBuffer->Add(constraint);

			// 差をそのまま拘束Cの結果とする
			constraint.error = diff.z;


			// ヤコビアンの計算
			constraint.jacobian[0] = Vector3::FORWARD;
			constraint.jacobian[1] = Vector3::Cross(rA, Vector3::FORWARD);
			constraint.jacobian[2] = -Vector3::FORWARD;
			constraint.jacobian[3] = -Vector3::Cross(rB, Vector3::FORWARD);

			MakeConstraintInfo(constraint, pointConstraint);

			// 拘束として追加
			_constraintBuffer->Add(constraint);
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

			MakeConstraintInfo(constraint, distanceConstraint);

			// 拘束として追加
			_constraintBuffer->Add(constraint);
		}
	}
}

// ヒンジ拘束の解く用の拘束構造体を作る
void ConstraintBuildSystem::BuildHingeConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer)
{

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
		if (!angleLimitPointConstraint.directionEndPoints.size() <= 1)
		{
			continue;
		}

		// 基準点となるボディから位置を持ってくる。
		uint32_t basePointIndex{ _solverBodyBuffer->GetIndex(angleLimitPointConstraint.directionEndPoints[0].transformID) };
		const SolverBody& solverBodyBase{ _solverBodyBuffer->Get(basePointIndex) };
		Vector3 basePoint{ solverBodyBase.position + solverBodyBase.rotation.Rotate(angleLimitPointConstraint.directionEndPoints[0].localPosition) };

		for (int i{ 1 }; i < angleLimitPointConstraint.directionEndPoints.size(); i++)
		{

		}
	}
}

// 角度制限付きヒンジ拘束の解く用の拘束構造体を作る
void ConstraintBuildSystem::BuildAngleLimitHingeConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer)
{

}


template<class T>
void ConstraintBuildSystem::MakeConstraintInfo(Constraint& _constraint, const T& _base)
{
	float deltaTime{ TimeManager::GetFixedDeltaTime() };

	float denominator = _base.tuning.damping + deltaTime * _base.tuning.stiffness;

	_constraint.softness = 1.0f / denominator;

	float erp{ deltaTime * _base.tuning.stiffness / denominator };

	_constraint.bias = erp / deltaTime * _constraint.error;

	_constraint.targetVelocity = 0.0f;

	_constraint.minLambda = -_base.tuning.maxForce * deltaTime;
	_constraint.maxLambda = _base.tuning.maxForce * deltaTime;
}