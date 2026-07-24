#include "ServiceLocator.h"

#include "ConstraintDebugRenderSystem.h"

void ConstraintDebugRenderSystem::Render(ConstraintStorage* _constraintStorage, PhysicsTransformStorage* _transformStorage)
{
	PointConstraintRender(_constraintStorage, _transformStorage);
	DistanceConstraintRender(_constraintStorage, _transformStorage);
}

void ConstraintDebugRenderSystem::PointConstraintRender(ConstraintStorage* _constraintStorage, PhysicsTransformStorage* _transformStorage)
{
	for (auto& pointConstraint : _constraintStorage->pointConstraintStorage->constraints)
	{
		// ポイントが2つ以上じゃないと拘束なんて発生しない
		if (pointConstraint.endPoints.size() <= 1)
		{
			continue;
		}

		for (int i{ 0 }; i < pointConstraint.endPoints.size(); i++)
		{
			// 基準点となる位置を持ってくる。
			uint32_t basePointIndex{ _transformStorage->GetDenseIndex(pointConstraint.endPoints[i].transformID) };
			Vector3 basePoint{ _transformStorage->GetPosition(basePointIndex) + _transformStorage->rotation[basePointIndex].Rotate(pointConstraint.endPoints[i].localPoint) };

			ServiceLocator::GetRenderer()->DrawSphereMesh(basePoint, 3.0f, Color{ 0,255,0 });
		}
	}
}

void ConstraintDebugRenderSystem::DistanceConstraintRender(ConstraintStorage* _constraintStorage, PhysicsTransformStorage* _transformStorage)
{
	for (auto& pointConstraint : _constraintStorage->distanceConstraintStorage->constraints)
	{
		// ポイントが2つ以上じゃないと拘束なんて発生しない
		if (pointConstraint.endPoints.size() <= 1)
		{
			continue;
		}

		// 基準点となる位置を持ってくる。
		uint32_t basePointIndex{ _transformStorage->GetDenseIndex(pointConstraint.endPoints[0].transformID) };
		Vector3 basePoint{ _transformStorage->GetPosition(basePointIndex) + _transformStorage->rotation[basePointIndex].Rotate(pointConstraint.endPoints[0].localPoint) };

		for (int i{ 1 }; i < pointConstraint.endPoints.size(); i++)
		{
			// 対象点となる位置を持ってくる。
			uint32_t pointIndex{ _transformStorage->GetDenseIndex(pointConstraint.endPoints[i].transformID) };
			Vector3 point{ _transformStorage->GetPosition(pointIndex) + _transformStorage->rotation[pointIndex].Rotate(pointConstraint.endPoints[i].localPoint) };

			ServiceLocator::GetRenderer()->DrawLine(basePoint, point, Color{ 0,255,0 });
		}
	}
}
