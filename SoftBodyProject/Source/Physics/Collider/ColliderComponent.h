#pragma once

#include "AABBBroadPhaseCollider.h"

#ifdef _DEBUG
#include "Color.h"
#endif // DEBUG

struct ColliderComponent
{
public:
	// コンストラクタ
	ColliderComponent() = default;
	// ブロードフェーズ用AABBの(XYZ軸ごとの)最小値
	const Vector3& GetBroadMin() { return aabb.min; }
	// ブロードフェーズ用AABBの(XYZ軸ごとの)最大値
	const Vector3& GetBroadMax() { return aabb.max; }
#ifdef _DEBUG
	// 色取得
	const Color& GetColor() { return color; }
	// 色セット
	void SetColor(const Color& _color) { color = _color; }
#endif // DEBUG
	/// <summary>
	/// サポート関数
	/// </summary>
	/// <param name="dir">方角</param>
	/// <returns>中心を原点としたときの点</returns>
	virtual Vector3 Support(const Vector3& dir) const = 0;
	// 仮想デストラクタ
	virtual ~ColliderComponent() = default;
protected:
	// ブロードフェーズ
	AABBBroadPhaseCollider aabb;
	// デバッグの色
#ifdef _DEBUG
	Color color;
#endif // _DEBUG
};
