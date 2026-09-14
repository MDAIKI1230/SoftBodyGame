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
	LimitedBallJointConstraintRender(_transformStorage, _constraintStorage);
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
			Vector3 basePoint{ _transformStorage->GetPosition(basePointTransID) + _transformStorage->GetRotation(basePointTransID).Rotate(pointConstraint.endPoints[i].localPosition) };

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
		Vector3 basePoint{ _transformStorage->GetPosition(basePointID) + _transformStorage->GetRotation(basePointID).Rotate(pointConstraint.endPoints[0].localPosition) };

		for (int i{ 1 }; i < pointConstraint.endPoints.size(); i++)
		{
			// 対象点となる位置を持ってくる。
			PhysicsTransformID pointTransID{ pointConstraint.endPoints[i].transformID };
			Vector3 point{ _transformStorage->GetPosition(pointTransID) + _transformStorage->GetRotation(pointTransID).Rotate(pointConstraint.endPoints[i].localPosition) };

			Renderer::DrawLine(basePoint, point, Color{ 0,1.0f,0 });
		}
	}
}

// ヒンジ拘束描画
void ConstraintDebugRenderingSystem::HingeConstraintRender(PhysicsTransformStorage* _transformStorage, ConstraintStorage* _constraintStorage)
{
	for (auto& hingeConstraint : _constraintStorage->GetHingeConstraintRange())
	{
		// 基準点を中心に円を書く
		const EndPointFrame& ownedPoint{ hingeConstraint.ownerEndPoint };

		PhysicsTransformID basePointTransID{ ownedPoint.transformID };

		const Quaternion& ownerRotation{ _transformStorage->GetRotation(basePointTransID) };

		Vector3 basePointPos{ _transformStorage->GetPosition(basePointTransID) + ownerRotation.Rotate(ownedPoint.localPosition) };
		Vector3 baseDirection{ ownedPoint.localRotation.Rotate(Vector3::UP) };
		baseDirection = ownerRotation.Rotate(baseDirection);

		Renderer::DrawRing(basePointPos, baseDirection.Normalized(), 20.0f, 10, Color{ 0,1.0f,0 });
		// 軸がわかりやすいように線を書く
		Renderer::DrawLine(basePointPos - baseDirection * 20.0f, basePointPos + baseDirection * 20.0f, Color{ 0,1.0f,0 });

		// ポイントごとに軸を線で書く
		for (auto& point : hingeConstraint.endPoints)
		{
			PhysicsTransformID pointOwnerTransformID{ point.transformID };

			const Quaternion& pointOwnerRotation{ _transformStorage->GetRotation(pointOwnerTransformID) };

			Vector3 pointPos{ _transformStorage->GetPosition(pointOwnerTransformID) + pointOwnerRotation.Rotate(point.localPosition) };
			Vector3 pointDir{ point.localRotation.Rotate(Vector3::UP) };
			pointDir = pointOwnerRotation.Rotate(pointDir);

			Renderer::DrawLine(pointPos - pointDir * 20.0f, pointPos + pointDir * 20.0f, Color{ 0,1.0f,0 });
		}
	}
}

// 角度制限付き点拘束描画
void ConstraintDebugRenderingSystem::AngleLimitPointConstraintRender(PhysicsTransformStorage* _transformStorage, ConstraintStorage* _constraintStorage)
{
	for (auto& angleLimitPointConstraint : _constraintStorage->GetAngleLimitPointConstraintRange())
	{
		if (angleLimitPointConstraint.endPoints.size() < 1)
		{
			continue;
		}

		const EndPointFrame& basePoint{ angleLimitPointConstraint.ownerEndPoint };
		PhysicsTransformID basePointTransID{ basePoint.transformID };
		const Quaternion& baseOwnedRotation{ _transformStorage->GetRotation(basePointTransID) };

		Vector3 basePosition{ _transformStorage->GetPosition(basePointTransID) + baseOwnedRotation.Rotate(basePoint.localPosition) };
		Vector3 baseDirection{ baseOwnedRotation.Rotate(basePoint.localRotation.Rotate(Vector3::UP)) };

		baseDirection.Normalize();

		float length{ 30.0f };

		Renderer::DrawSphereMesh(basePosition, 3.0f, Color{ 0.0f,1.0f,0.0f });
		Renderer::DrawLine(basePosition, basePosition + baseDirection * length, Color{ 0.0f,1.0f,0.0f });

		// 最小角度の描画(角度がほぼ0以下なら線/角度がほぼ直角なら円/それ以外は円錐)
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

		// 最大角度の描画(角度がほぼ0以下なら線/角度がほぼ直角なら円/それ以外は円錐)
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
		for (const auto& point: angleLimitPointConstraint.endPoints)
		{
			PhysicsTransformID pointTransID{ point.transformID };
			const Quaternion& rotation{ _transformStorage->GetRotation(pointTransID) };

			Vector3 position{ _transformStorage->GetPosition(pointTransID) + rotation.Rotate(point.localPosition) };
			Vector3 direction{ rotation.Rotate(point.localRotation.Rotate(Vector3::RIGHT)) };

			Renderer::DrawLine(position, position + direction * length, Color{ 0.0f,1.0f,0.0f });
		}
	}
}

// 角度制限付きヒンジ拘束描画
void ConstraintDebugRenderingSystem::AngleLimitHingeConstraintRender(PhysicsTransformStorage* _transformStorage, ConstraintStorage* _constraintStorage)
{
	for (auto& angleLimitHingeConstraint : _constraintStorage->GetAngleLimitHingeConstraintRange())
	{
		const EndPointFrame& basePoint{ angleLimitHingeConstraint.ownerEndPoint };

		// 基準点となる位置を持ってくる。
		PhysicsTransformID basePointTransID{ basePoint.transformID };

		const Quaternion& baseRotation{ _transformStorage->GetRotation(basePointTransID) };

		Vector3 basePosition{ _transformStorage->GetPosition(basePointTransID) + baseRotation.Rotate(basePoint.localPosition) };
		Vector3 baseAxis{ baseRotation.Rotate(basePoint.localRotation.Rotate(Vector3::UP)) };
		Vector3 baseReference{ baseRotation.Rotate(basePoint.localRotation.Rotate(Vector3::RIGHT)) };

		Renderer::DrawSector(
			basePosition, baseAxis, baseReference,
			30.0f,
			angleLimitHingeConstraint.angleMin, angleLimitHingeConstraint.angleMax,
			32, Color{ 0.0f, 1.0f, 0.0f });

		// ポイントないなら終了
		if (angleLimitHingeConstraint.endPoints.size() < 1)
		{
			continue;
		}

		for (const auto& point : angleLimitHingeConstraint.endPoints)
		{
			// 基準点となる位置を持ってくる。
			PhysicsTransformID pointTransID{ point.transformID };

			const Quaternion& rotation{ _transformStorage->GetRotation(pointTransID) };

			Vector3 position{ _transformStorage->GetPosition(pointTransID) + rotation.Rotate(point.localPosition) };
			Vector3 direction{ rotation.Rotate(point.localRotation.Rotate(Vector3::RIGHT)) };

			Renderer::DrawLine(position, position + direction * 30.0f, Color{ 0.0f, 1.0f, 0.0f });
		}
	}
}

// SwingTwist拘束描画
void ConstraintDebugRenderingSystem::LimitedBallJointConstraintRender(PhysicsTransformStorage* _transformStorage, ConstraintStorage* _constraintStorage)
{
	for (auto& limitedBallJointConstraint : _constraintStorage->GetLimitedBallJointConstraintRange())
	{
		if (limitedBallJointConstraint.endPoints.size() < 1)
		{
			continue;
		}

		const EndPointFrame& basePoint{ limitedBallJointConstraint.ownerEndPoint };
		PhysicsTransformID basePointTransID{ basePoint.transformID };
		const Quaternion& baseOwnedRotation{ _transformStorage->GetRotation(basePointTransID) };

		Vector3 basePosition{ _transformStorage->GetPosition(basePointTransID) + baseOwnedRotation.Rotate(basePoint.localPosition) };
		Vector3 baseDirection{ baseOwnedRotation.Rotate(basePoint.localRotation.Rotate(Vector3::UP)) };
		Vector3 baseReference{ baseOwnedRotation.Rotate(basePoint.localRotation.Rotate(Vector3::RIGHT)) };

		baseDirection.Normalize();

		float length{ 30.0f };

		Renderer::DrawSphereMesh(basePosition, 3.0f, Color{ 0.0f,1.0f,0.0f });
		Renderer::DrawLine(basePosition, basePosition + baseDirection * length, Color{ 0.0f,1.0f,0.0f });

		// Swing角度の描画(角度がほぼ0以下なら線/角度がほぼ直角なら円/それ以外は円錐)
		float maxAxisLength{ std::cos(limitedBallJointConstraint.swingAngle) * length };
		float maxRadius{ std::abs(std::sin(limitedBallJointConstraint.swingAngle) * length) };
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
		for (const auto& point : limitedBallJointConstraint.endPoints)
		{
			PhysicsTransformID pointTransID{ point.transformID };
			const Quaternion& rotation{ _transformStorage->GetRotation(pointTransID) };

			Vector3 position{ _transformStorage->GetPosition(pointTransID) + rotation.Rotate(point.localPosition) };
			Vector3 direction{ rotation.Rotate(point.localRotation.Rotate(Vector3::UP)) };

			Vector3 reference{ rotation.Rotate(point.localRotation.Rotate(Vector3::RIGHT)) };

			Renderer::DrawLine(position, position + reference * length, Color{ 0.0f, 1.0f, 0.0f });

			Renderer::DrawLine(position, position + direction * length, Color{ 0.0f,1.0f,0.0f });

			// Twist角度の描画
			Renderer::DrawSector(
				position, direction, baseReference,
				30.0f,
				0.0f, limitedBallJointConstraint.twistAngle,
				32, Color{ 0.0f, 1.0f, 0.0f });
		}
	}
}