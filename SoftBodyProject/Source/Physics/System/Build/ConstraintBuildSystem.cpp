#include "ConstraintBuildSystem.h"

void ConstraintBuildSystem::FixedUpdate(PhysicsTransformStorage* _transformStorage, ConstraintStorage* _constraintStorage, ConstraintBuffer* _constraintBuffer)
{
	BuildPointConstraint(_transformStorage, _constraintStorage, _constraintBuffer);
}

void ConstraintBuildSystem::BuildPointConstraint(PhysicsTransformStorage* _transformStorage, ConstraintStorage* _constraintStorage, ConstraintBuffer* _constraintBuffer)
{
	for (auto& pointConstraint : _constraintStorage->pointConstraintStorage->constraints)
	{
		// ポイントが2つ以上じゃないと拘束なんて発生しない
		if (pointConstraint.endPoints.size() <= 1)
		{
			continue;
		}
		// 基準点となる最初を持ってくる。
		uint32_t basePointIndex{ _transformStorage->GetDenseIndex(pointConstraint.endPoints[0].transformID) };
		Vector3 basePoint{ _transformStorage->position[basePointIndex] + pointConstraint.endPoints[0].localPoint };

		for (int i{ 0 }; i < pointConstraint.endPoints.size(); i++)
		{
			Constraint constraint;

			// 対象の位置を取得
			uint32_t pointIndex{ _transformStorage->GetDenseIndex(pointConstraint.endPoints[i].transformID) };
			Vector3 point{ _transformStorage->position[pointIndex] + pointConstraint.endPoints[i].localPoint };
		}
	}
}
