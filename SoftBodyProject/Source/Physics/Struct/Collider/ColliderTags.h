#pragma once

#include "MDMath.h"

#include "PhysicsTransformStorage.h"
#include "ColliderStorage.h"

#include "Manifold.h"

namespace ColliderTag
{
	// 球
	struct SphereTag
	{
		static Vector3 Support(ColliderStorage* _colliderStorage, ColliderID _id, PhysicsTransformStorage* _transformStorage, const Vector3& _dir)
		{
			uint32_t transformIndex{ _transformStorage->GetDenseIndex(_colliderStorage->GetTransformID(_id)) };
			return _dir.Normalized() * _colliderStorage->sphereStorage->radius[_colliderStorage->GetDenseIndex(_id)];
		}

		static Manifold MakeManifold(const Vector3& _normal, float _penetration)
		{

		}
	};

	// 箱
	struct BoxTag
	{
		static Vector3 Support(ColliderStorage* _colliderStorage, ColliderID _id, PhysicsTransformStorage* _transformStorage, const Vector3& _dir)
		{
			Vector3 halfScale{ _colliderStorage->boxStorage->scale[_colliderStorage->GetDenseIndex(_id)] * 0.5f };
			uint32_t transformIndex{ _transformStorage->GetDenseIndex(_colliderStorage->GetTransformID(_id)) };
			halfScale = SIMDVectorMath::Mul(halfScale, _transformStorage->scale[transformIndex]);
			Quaternion& rot{ _transformStorage->rotation[transformIndex] };
			Vector3 candidates[8]
			{
				rot.Rotate(halfScale),
				rot.Rotate(Vector3{halfScale.x,halfScale.y,-halfScale.z}),
				rot.Rotate(Vector3{halfScale.x,-halfScale.y,halfScale.z}),
				rot.Rotate(Vector3{-halfScale.x,halfScale.y,halfScale.z}),
				rot.Rotate(halfScale),
				rot.Rotate(Vector3{halfScale.x,halfScale.y,-halfScale.z}),
				rot.Rotate(Vector3{halfScale.x,-halfScale.y,halfScale.z}),
				rot.Rotate(Vector3{-halfScale.x,halfScale.y,halfScale.z})
			};

			char maxIndex{ 0 };
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
}
