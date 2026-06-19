#pragma once

#include "MDMath.h"

#include "SphereColliderComponentStorage.h"

namespace ColliderTag
{
	// 球
	struct SphereTag
	{
		static Vector3 Support(SphereColliderComponentStorage* _storage, int _index, const Vector3& _dir)
		{
			return _dir.Normalized() * _storage->Get(_index)->GetRadius();
		}
	};
}
