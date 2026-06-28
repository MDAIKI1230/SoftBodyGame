#pragma once

#include "MDMath.h"

#include "TransformComponent.h"
#include "SphereColliderStorage.h"
#include "BoxColliderStorage.h"

namespace ColliderTag
{
	// 球
	struct SphereTag
	{
		static Vector3 Support(SphereColliderStorage* _storage, size_t _index, TransformComponent* _trans, const Vector3& _dir)
		{
			return _dir.Normalized() * _storage->radius[_index] + _trans->GetPosition();
		}
	};

	// 箱
	struct BoxTag
	{
		static Vector3 Support(BoxColliderStorage* _storage, size_t _index, TransformComponent* _trans, const Vector3& _dir)
		{
			Vector3 halfScale{ _storage->scale[_index] * 0.5f };
			Vector3 pos{ _trans->GetPosition() };
			Quaternion rot{ _trans->GetRotation() };
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
