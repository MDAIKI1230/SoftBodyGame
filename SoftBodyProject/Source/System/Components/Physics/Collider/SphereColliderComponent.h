#pragma once

struct SphereColliderComponent
{
public:
	// コンストラクタ
	SphereColliderComponent() = default;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_r">半径</param>
	SphereColliderComponent(float _r) :
		r{ _r }
	{
	}

	// 半径セット
	void SetRadius(float _r)
	{
		r = _r;
	}

	// 半径取得
	float GetRadius()
	{
		return r;
	}
private:
	// 半径
	float r{ 0 };
};