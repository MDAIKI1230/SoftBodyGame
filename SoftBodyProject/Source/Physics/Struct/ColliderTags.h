#pragma once

#include "MDMath.h"

#include "SphereColliderStorage.h"
#include "BoxColliderStorage.h"

namespace ColliderTag
{
	// 球
	struct SphereTag
	{
		static Vector3 Support(SphereColliderStorage* _storage, size_t _index, const Vector3& _dir)
		{
			return _dir.Normalized() * _storage->radius[_index];
		}
	};

	// 箱
	struct BoxTag
	{
		static Vector3 Support(BoxColliderStorage* _storage, size_t _index, const Vector3& _dir)
		{

			return Vector3::UP;
		}
	};
}
