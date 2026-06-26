#pragma once

#include "ColliderComponent.h"

struct BoxColliderComponent :public ColliderComponent
{
public:
	// ---コンストラクタ---

	// コンストラクタ
	BoxColliderComponent(int _handle) :
		ColliderComponent{ _handle }
	{
	}
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_handle">ID</param>
	/// <param name="_width">X方向の長さ</param>
	/// <param name="_height">Y方向の長さ</param>
	/// <param name="_depth">Z方向の長さ</param>
	BoxColliderComponent(int _handle, float _width, float _height, float _depth);

	// X方向の長さ取得
	float GetWidth();
	// X方向の長さ変更
	void SetWidth(float _width);
	// Y方向の長さ取得
	float GetHeight();
	// Y方向の長さ変更
	void SetHeight(float _heibht);
	// Z方向の長さ取得
	float GetDepth();
	// Z方向の長さ変更
	void SetDepth(float _depth);
};
