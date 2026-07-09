#include "ConstraintBuildSystem.h"

void ConstraintBuildSystem::FixedUpdate(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer)
{
	BuildPointConstraint(_constraintStorage, _solverBodyBuffer, _constraintBuffer);
}

void ConstraintBuildSystem::BuildPointConstraint(ConstraintStorage* _constraintStorage, SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer)
{
	// 拘束が存在するかチェック
	if (_constraintStorage->pointConstraintStorage->constraints.size() <= 0)
	{
		return;
	}
	for (auto& pointConstraint : _constraintStorage->pointConstraintStorage->constraints)
	{
		// ポイントが2つ以上じゃないと拘束なんて発生しない
		if (pointConstraint.endPoints.size() <= 1)
		{
			continue;
		}
		// 基準点となるボディから位置を持ってくる。
		uint32_t basePointIndex{ _solverBodyBuffer->bodyMap[pointConstraint.endPoints[0].transformID] };
		SolverBody& solverBodyBase{ _solverBodyBuffer->solverBodies[basePointIndex] };
		Vector3 basePoint{ solverBodyBase.position + pointConstraint.endPoints[0].localPoint };

		for (int i{ 0 }; i < pointConstraint.endPoints.size(); i++)
		{
			Constraint constraint;

			// 対象の位置を取得
			uint32_t pointIndex{ _solverBodyBuffer->bodyMap[pointConstraint.endPoints[i].transformID] };
			SolverBody solverBody{ _solverBodyBuffer->solverBodies[pointIndex] };
			Vector3 point{ solverBody.position + pointConstraint.endPoints[i].localPoint };

			constraint.solverBodyAIndex = basePointIndex;
			constraint.solverBodyBIndex = pointIndex;

			// 差
			Vector3 diff{ basePoint - point };
			// 差をそのまま拘束Cの結果とする
			constraint.constraintError = diff.Length();

			// 正規化して法線にする
			diff.Normalize();

			// 作用する点とのベクトル
			Vector3 rB{ basePoint - solverBody.position };

			// ヤコビアンの計算
			constraint.jacobian[0] = diff;
			constraint.jacobian[1] = Vector3::ZERO;
			constraint.jacobian[2] = -diff;
			constraint.jacobian[3] = -Vector3::Cross(rB, diff);

			// 拘束として追加
			_constraintBuffer->constraints.push_back(constraint);
		}
	}
}
