#include <algorithm>

#include "Renderer.h"

#include "ConstraintDebugRenderingSystem.h"

void ConstraintDebugRenderingSystem::Render(PhysicsTransformStorage* _transformStorage, ConstraintStorage* _constraintStorage)
{
	PointConstraintRender(_transformStorage, _constraintStorage);
	DistanceConstraintRender(_transformStorage, _constraintStorage);
	HingeConstraintRender(_transformStorage, _constraintStorage);
	AngleLimitPointConstraintRender(_transformStorage, _constraintStorage);
	AngleLimitHingeConstraintRender(_transformStorage, _constraintStorage);
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

// ヒンジ拘束描画
void ConstraintDebugRenderingSystem::HingeConstraintRender(PhysicsTransformStorage* _transformStorage, ConstraintStorage* _constraintStorage)
{
	for (auto& hingeConstraint : _constraintStorage->GetHingeConstraintRange())
	{
		const DirectionEndPoint& point{ hingeConstraint.ownerEndPoint };

		// 基準点となる位置を持ってくる。
		PhysicsTransformID basePointTransID{ point.transformID };

		const Quaternion& rotation{ _transformStorage->GetRotation(basePointTransID) };

		Vector3 basePoint{ _transformStorage->GetPosition(basePointTransID) + rotation.Rotate(point.localPosition) };
		Vector3 baseDirection{ rotation.Rotate(point.localDirection) };

		Renderer::DrawRing(basePoint, baseDirection.Normalized(), 20.0f, 10.0f, Color{ 0,1.0f,0 });
	}
}

// 角度制限付き点拘束描画
void ConstraintDebugRenderingSystem::AngleLimitPointConstraintRender(PhysicsTransformStorage* _transformStorage, ConstraintStorage* _constraintStorage)
{
	for (auto& angleLimitPointConstraint : _constraintStorage->GetAngleLimitPointConstraintRange())
	{
		if (angleLimitPointConstraint.directionEndPoints.size() <= 1)
		{
			continue;
		}

		const DirectionEndPoint& basePoint{ angleLimitPointConstraint.directionEndPoints[0] };
		PhysicsTransformID basePointTransID{ basePoint.transformID };
		const Quaternion& baseRotation{ _transformStorage->GetRotation(basePointTransID) };

		Vector3 basePosition{ _transformStorage->GetPosition(basePointTransID) + baseRotation.Rotate(basePoint.localPosition) };
		Vector3 baseDirection{ baseRotation.Rotate(basePoint.localDirection) };

		if (baseDirection.LengthSqr() <= MathConstants::EPSILON * MathConstants::EPSILON)
		{
			continue;
		}

		baseDirection.Normalize();

		float length{ 30.0f };

		Renderer::DrawSphereMesh(basePosition, 3.0f, Color{ 0.0f,1.0f,0.0f });
		Renderer::DrawLine(basePosition, basePosition + baseDirection * length, Color{ 0.0f,1.0f,0.0f });

		// 最小角度
		if (angleLimitPointConstraint.angleMin > MathConstants::EPSILON)
		{
			float minAxisLength{ std::cos(angleLimitPointConstraint.angleMin) * length };
			float minRadius{ std::abs(std::sin(angleLimitPointConstraint.angleMin) * length) };
			Vector3 minBottomPosition{ basePosition + baseDirection * minAxisLength };

			if (minRadius <= MathConstants::EPSILON)
			{
				Renderer::DrawLine(basePosition, minBottomPosition, Color{ 0.0f,1.0f,0.0f });
			}
			else if (std::abs(minAxisLength) <= MathConstants::EPSILON)
			{
				Renderer::DrawRing(basePosition, baseDirection, minRadius, 32, Color{ 0.0f,1.0f,0.0f });
			}
			else
			{
				Renderer::DrawCone(basePosition, minBottomPosition, minRadius, 32, Color{ 0.0f,1.0f,0.0f });
			}
		}

		// 最大角度
		float maxAxisLength{ std::cos(angleLimitPointConstraint.angleMax) * length };
		float maxRadius{ std::abs(std::sin(angleLimitPointConstraint.angleMax) * length) };
		Vector3 maxBottomPosition{ basePosition + baseDirection * maxAxisLength };

		if (maxRadius <= MathConstants::EPSILON)
		{
			Renderer::DrawLine(basePosition, maxBottomPosition, Color{ 0.0f,1.0f,0.0f });
		}
		else if (std::abs(maxAxisLength) <= MathConstants::EPSILON)
		{
			Renderer::DrawRing(basePosition, baseDirection, maxRadius, 32, Color{ 0.0f,1.0f,0.0f });
		}
		else
		{
			Renderer::DrawCone(basePosition, maxBottomPosition, maxRadius, 32, Color{ 0.0f,1.0f,0.0f });
		}

		// 対象側の現在方向
		for (int i{ 1 }; i < angleLimitPointConstraint.directionEndPoints.size(); i++)
		{
			const DirectionEndPoint& point{ angleLimitPointConstraint.directionEndPoints[i] };
			PhysicsTransformID pointTransID{ point.transformID };
			const Quaternion& rotation{ _transformStorage->GetRotation(pointTransID) };

			Vector3 position{ _transformStorage->GetPosition(pointTransID) + rotation.Rotate(point.localPosition) };
			Vector3 direction{ rotation.Rotate(point.localDirection) };

			if (direction.LengthSqr() <= MathConstants::EPSILON * MathConstants::EPSILON)
			{
				continue;
			}

			direction.Normalize();

			float sinAngle{ Vector3::Cross(baseDirection, direction).Length() };
			float cosAngle{ std::clamp(Vector3::Dot(baseDirection, direction), -1.0f, 1.0f) };
			float angle{ std::atan2(sinAngle, cosAngle) };

			Renderer::DrawLine(position, position + direction * length, Color{ 0.0f,1.0f,0.0f });
		}
	}
}

// 角度制限付きヒンジ拘束描画
void ConstraintDebugRenderingSystem::AngleLimitHingeConstraintRender(PhysicsTransformStorage* _transformStorage, ConstraintStorage* _constraintStorage)
{
	for (auto& angleLimitHingeConstraint : _constraintStorage->GetAngleLimitHingeConstraintRange())
	{
		const AngleLimitHingeEndPoint& basePoint{ angleLimitHingeConstraint.ownerEndPoint };

		// 基準点となる位置を持ってくる。
		PhysicsTransformID basePointTransID{ basePoint.transformID };

		const Quaternion& baseRotation{ _transformStorage->GetRotation(basePointTransID) };

		Vector3 basePosition{ _transformStorage->GetPosition(basePointTransID) + baseRotation.Rotate(basePoint.localPosition) };
		Vector3 baseAxis{ baseRotation.Rotate(basePoint.localAxis) };
		Vector3 baseReference{ baseRotation.Rotate(basePoint.localReferenceDirection) };

		Renderer::DrawSector(
			basePosition, baseAxis, baseReference,
			30.0f,
			angleLimitHingeConstraint.angleMin, angleLimitHingeConstraint.angleMax,
			32, Color{ 0.0f, 1.0f, 0.0f });

		// ポイントないなら終了
		if (angleLimitHingeConstraint.angleLimitHingeEndPoints.size() < 1)
		{
			continue;
		}

		for (int i{ 0 }; i < angleLimitHingeConstraint.angleLimitHingeEndPoints.size(); i++)
		{
			const AngleLimitHingeEndPoint& point{ angleLimitHingeConstraint.angleLimitHingeEndPoints[i] };

			// 基準点となる位置を持ってくる。
			PhysicsTransformID pointTransID{ point.transformID };

			const Quaternion& rotation{ _transformStorage->GetRotation(pointTransID) };

			Vector3 position{ _transformStorage->GetPosition(pointTransID) + rotation.Rotate(point.localPosition) };
			Vector3 direction{ rotation.Rotate(point.localReferenceDirection) };

			Renderer::DrawLine(position, position + direction * 30.0f, Color{ 0.0f, 1.0f, 0.0f });
		}
	}
}
