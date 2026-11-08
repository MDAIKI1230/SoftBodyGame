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
			Vector3 position{ _transformStorage->GetPosition(transformID) + rotation.Rotate(_colliderStorage->GetSphereColliderOffsetPosition(_id)) };
			return _dir.Normalized() * _colliderStorage->GetSphereColliderRadius(_id) + position;
		}
	};

	// 箱
	struct BoxTag
	{
		static Vector3 Support(ColliderStorage* _colliderStorage, ColliderID _id, PhysicsTransformStorage* _transformStorage, const Vector3& _dir)
		{
			Vector3 halfScale{ _colliderStorage->GetBoxColliderScale(_id) * 0.5f };
			PhysicsTransformID transformID{ _colliderStorage->GetTransformID(_id) };
			halfScale = SIMDVectorMath::Mul(halfScale, _transformStorage->GetScale(transformID));
			Quaternion transformRot{ _transformStorage->GetRotation(transformID) };
			Vector3 pos{ _transformStorage->GetPosition(transformID) + transformRot.Rotate(_colliderStorage->GetBoxColliderOffsetPosition(_id)) };
			Quaternion rot{ transformRot * _colliderStorage->GetBoxColliderOffsetRotation(_id) };
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
			const Vector3& position{
				_transformStorage->GetPosition(transformID) +
				transformRot.Rotate(_colliderStorage->GetCapsuleColliderOffsetPosition(_id)) };
			const Quaternion& rotation{ transformRot * _colliderStorage->GetCapsuleColliderOffsetRotation(_id) };
			const Vector3& scale{ _transformStorage->GetScale(transformID) };

			float halfHeight{
				std::abs(_colliderStorage->GetCapsuleColliderHeight(_id) * scale.y) * 0.5f
			};

			float radius{ std::abs(_colliderStorage->GetCapsuleColliderRadius(_id)) };

			Vector3 direction{ _dir.Normalized() };
			Vector3 capsuleAxis{ rotation.Rotate(Vector3::UP) };

			float endpointSign{Vector3::Dot(capsuleAxis, direction) >= 0.0f ? 1.0f : -1.0f};

			Vector3 segmentEndpoint{ position + capsuleAxis * halfHeight * endpointSign };

			return segmentEndpoint + direction * radius;
		}
	};
}
