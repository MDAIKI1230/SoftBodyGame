#pragma once

#include "MDMath.h"

#include "PhysicsTransformStorage.h"
#include "SphereColliderStorage.h"
#include "BoxColliderStorage.h"

namespace ColliderTag
{
	// 球
	struct SphereTag
	{
		static Vector3 Support(SphereColliderStorage* _colliderStorage, size_t _index, PhysicsTransformStorage* _transformStorage, const Vector3& _dir)
		{
			uint32_t transformIndex{ _transformStorage->GetDenseIndex(_colliderStorage->transformID[_index]) };
			return _dir.Normalized() * _colliderStorage->radius[_index] + _transformStorage->position[transformIndex];
		}
	};

	// 箱
	struct BoxTag
	{
		static Vector3 Support(BoxColliderStorage* _colliderStorage, size_t _index, PhysicsTransformStorage* _transformStorage, const Vector3& _dir)
		{
			Vector3 halfScale{ _colliderStorage->scale[_index] * 0.5f };
			uint32_t transformIndex{ _transformStorage->GetDenseIndex(_colliderStorage->transformID[_index]) };
			Vector3& pos{ _transformStorage->position[transformIndex] };
			Quaternion& rot{ _transformStorage->rotation[transformIndex] };
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

			char maxIndex{ 0 };
			float best{ FLT_MIN };

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
