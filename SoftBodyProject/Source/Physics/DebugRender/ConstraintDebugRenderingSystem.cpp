#include "Renderer.h"

#include "ConstraintDebugRenderingSystem.h"

void ConstraintDebugRenderingSystem::Render(PhysicsTransformStorage* _transformStorage, ConstraintStorage* _constraintStorage)
{
	PointConstraintRender(_transformStorage, _constraintStorage);
	DistanceConstraintRender(_transformStorage, _constraintStorage);
}

void ConstraintDebugRenderingSystem::PointConstraintRender(PhysicsTransformStorage* _transformStorage, ConstraintStorage* _constraintStorage)
{
	for (auto& pointConstraint : _constraintStorage->GetPointConstraintRange())
	{
		// ポイントが2つ以上じゃないと拘束なんて発生しない
		if (pointConstraint.endPoints.size() <= 1)
		{
			continue;
		}

		for (int i{ 0 }; i < pointConstraint.endPoints.size(); i++)
		{
			// 基準点となる位置を持ってくる。
			PhysicsTransformID basePointTransID{ pointConstraint.endPoints[i].transformID };
			Vector3 basePoint{ _transformStorage->GetPosition(basePointTransID) + _transformStorage->GetRotation(basePointTransID).Rotate(pointConstraint.endPoints[i].localPoint) };

			Renderer::DrawSphereMesh(basePoint, 3.0f, Color{ 0,1.0f,0 });
		}
	}
}

void ConstraintDebugRenderingSystem::DistanceConstraintRender(PhysicsTransformStorage* _transformStorage, ConstraintStorage* _constraintStorage)
{
	for (auto& pointConstraint : _constraintStorage->GetDistanceConstraintRange())
	{
		// ポイントが2つ以上じゃないと拘束なんて発生しない
		if (pointConstraint.endPoints.size() <= 1)
		{
			continue;
		}

		// 基準点となる位置を持ってくる。
		PhysicsTransformID basePointID{ pointConstraint.endPoints[0].transformID };
		Vector3 basePoint{ _transformStorage->GetPosition(basePointID) + _transformStorage->GetRotation(basePointID).Rotate(pointConstraint.endPoints[0].localPoint) };

		for (int i{ 1 }; i < pointConstraint.endPoints.size(); i++)
		{
			// 対象点となる位置を持ってくる。
			PhysicsTransformID pointTransID{ pointConstraint.endPoints[i].transformID };
			Vector3 point{ _transformStorage->GetPosition(pointTransID) + _transformStorage->GetRotation(pointTransID).Rotate(pointConstraint.endPoints[i].localPoint) };

			Renderer::DrawLine(basePoint, point, Color{ 0,1.0f,0 });
		}
	}
}
