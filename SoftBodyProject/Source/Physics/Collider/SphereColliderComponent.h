#pragma once

#include "ColliderComponent.h"

struct SphereColliderComponent :public ColliderComponent
{
public:
	// ---コンストラクタ---

	// デフォルトコンストラクタ
	SphereColliderComponent() = default;
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_r">半径</param>
	SphereColliderComponent(float _r) :
		r{ _r }
	{
	}

	// サポート関数
	Vector3 Support(const Vector3& dir) const override;

	// ---半径ゲッターセッター---

	// 半径セット
	void SetRadius(float _r) { r = _r; }
	// 半径取得
	float GetRadius() { return r; }
private:
	// 半径
	float r{ 0 };
};