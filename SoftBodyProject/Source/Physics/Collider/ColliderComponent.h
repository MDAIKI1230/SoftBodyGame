#pragma once

#include "AABBBroadPhaseCollider.h"

class ColliderComponent
{
public:
	// ブロードフェーズ用AABBの(XYZ軸ごとの)最小値
	const Vector3& GetBroadMin() { return aabb.min; }
	// ブロードフェーズ用AABBの(XYZ軸ごとの)最大値
	const Vector3& GetBroadMax() { return aabb.max; }
	// サポート関数
	virtual Vector3 Support(const Vector3& dir) = 0;
protected:
	AABBBroadPhaseCollider aabb;
};