#pragma once

#include "ColliderComponent.h"

struct SphereColliderComponent :public ColliderComponent
{
public:
	// ---コンストラクタ---

	// コンストラクタ
	SphereColliderComponent(int _handle) :
		ColliderComponent{ _handle }
	{
	}
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_r">半径</param>
	SphereColliderComponent(int _handle, float _r) :
		ColliderComponent{ _handle },
		r{ _r }
	{
		aabb.min = { -r,-r,-r };
		aabb.max = { r,r,r };
	}

	// ---半径ゲッターセッター---

	// 半径セット
	void SetRadius(float _r) { r = _r; }
	// 半径取得
	float GetRadius() { return r; }
private:
	// 半径
	float r{ 0 };
};
