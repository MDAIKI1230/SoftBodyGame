#pragma once

#include "MDMath.h"

#include "PhysicsTransformStorage.h"
#include "ColliderStorage.h"

namespace 
{
	// 球
	struct SphereTag
	{
		static Vector3 Support(ColliderStorage* _colliderStorage, ColliderID _id, PhysicsTransformStorage* _transformStorage, const Vector3& _dir)
		{
			PhysicsTransformID transformID{ _colliderStorage->GetTransformID(_id) };
			Quaternion rotation{ _transformStorage->GetRotation(transformID) };
			const Vector3& scale{ _transformStorage->GetScale(transformID) };
			Vector3 position{ _transformStorage->GetPosition(transformID) + rotation.Rotate(_colliderStorage->GetScaledSphereOffset(_id, scale)) };
			return _dir.Normalized() * _colliderStorage->GetScaledSphereRadius(_id, scale) + position;
		}
	};

	// 箱
	struct BoxTag
	{
		static Vector3 Support(ColliderStorage* _colliderStorage, ColliderID _id, PhysicsTransformStorage* _transformStorage, const Vector3& _dir)
		{
			PhysicsTransformID transformID{ _colliderStorage->GetTransformID(_id) };
			const Vector3& scale{ _transformStorage->GetScale(transformID) };
			Vector3 halfScale{ _colliderStorage->GetScaledBoxSize(_id, scale) * 0.5f };
			Quaternion transformRot{ _transformStorage->GetRotation(transformID) };
			Vector3 pos{ _transformStorage->GetPosition(transformID) + transformRot.Rotate(_colliderStorage->GetScaledBoxOffset(_id, scale)) };
			Quaternion rot{ transformRot };
			Vector3 candidates[8]
			{
				pos + rot.Rotate(halfScale),
				pos + rot.Rotate(Vector3{halfScale.x,halfScale.y,-halfScale.z}),
				pos + rot.Rotate(Vector3{halfScale.x,-halfScale.y,halfScale.z}),
				pos + rot.Rotate(Vector3{-halfScale.x,halfScale.y,halfScale.z}),
				pos - rot.Rotate(halfScale),
				pos - rot.Rotate(Vector3{halfScale.x,halfScale.y,-halfScale.z}),
				pos - rot.Rotate(Vector3{halfScale.x,-halfScale.y,halfScale.z}),
				pos - rot.Rotate(Vector3{-halfScale.x,halfScale.y,halfScale.z})
			};

			int maxIndex{ 0 };
			float best{ -FLT_MAX };

			for (int i{ 0 }; i < 8; i++)
			{
				float dot{ Vector3::Dot(candidates[i], _dir) };
				if (dot > best)
				{
					maxIndex = i;
					best = dot;
				}
			}

			return candidates[maxIndex];
		}
	};

	// カプセル
	struct CapsuleTag
	{
		static Vector3 Support(ColliderStorage* _colliderStorage, ColliderID _id, PhysicsTransformStorage* _transformStorage, const Vector3& _dir)
		{
			PhysicsTransformID transformID{ _colliderStorage->GetTransformID(_id) };

			Quaternion transformRot{ _transformStorage->GetRotation(transformID) };
			const Vector3& scale{ _transformStorage->GetScale(transformID) };
			const Vector3& position{
				_transformStorage->GetPosition(transformID) +
				transformRot.Rotate(_colliderStorage->GetScaledCapsuleOffset(_id, scale)) };
			const Quaternion& rotation{ transformRot };

			float halfHeight{
				_colliderStorage->GetScaledCapsuleHeight(_id, scale) * 0.5f
			};

			float radius{ _colliderStorage->GetScaledCapsuleRadius(_id, scale) };

			Vector3 direction{ _dir.Normalized() };
			Vector3 capsuleAxis{ rotation.Rotate(Vector3::UP) };

			float endpointSign{Vector3::Dot(capsuleAxis, direction) >= 0.0f ? 1.0f : -1.0f};

			Vector3 segmentEndpoint{ position + capsuleAxis * halfHeight * endpointSign };

			return segmentEndpoint + direction * radius;
		}
	};
}