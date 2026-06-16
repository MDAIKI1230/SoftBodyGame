#pragma once

#include "AABBBroadPhaseCollider.h"

struct ColliderComponent
{
public:
	// ブロードフェーズ用AABBの(XYZ軸ごとの)最小値
	const Vector3& GetBroadMin() { return aabb.min; }
	// ブロードフェーズ用AABBの(XYZ軸ごとの)最大値
	const Vector3& GetBroadMax() { return aabb.max; }
	/// <summary>
	/// サポート関数
	/// </summary>
	/// <param name="dir">方角</param>
	/// <returns>中心を原点としたときの点</returns>
	virtual Vector3 Support(const Vector3& dir) const = 0;
	// 仮想デストラクタ
	virtual ~ColliderComponent() = default;
protected:
	AABBBroadPhaseCollider aabb;
};