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
	BoxColliderComponent(int _handle, float _width, float _height, float _depth) :
		ColliderComponent{ _handle },
		width{ _width },
		height{ _height },
		depth{ _depth }
	{
	}

	BoxColliderComponent(int _handle, float _size) :
		ColliderComponent{ _handle },
		width{ _size },
		height{ _size },
		depth{ _size }
	{
	}

	// X方向の長さ取得
	float GetWidth() { return width; }
	// X方向の長さ変更
	void SetWidth(float _width) { width = _width; }
	// Y方向の長さ取得
	float GetHeight() { return height; }
	// Y方向の長さ変更
	void SetHeight(float _heibht) { height = _heibht; }
	// Z方向の長さ取得
	float GetDepth() { return depth; }
	// Z方向の長さ変更
	void SetDepth(float _depth) { depth = _depth; }
private:
	float width{ 0.0f }, height{ 0.0f }, depth{ 0.0f };
};
