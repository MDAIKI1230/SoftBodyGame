#pragma once

#include "ColliderComponent.h"

struct BoxColliderComponent :public ColliderComponent
{
public:
	// ---コンストラクタ---

	// コンストラクタ
	BoxColliderComponent(EntityID _entity);
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_handle">ID</param>
	/// <param name="_width">X方向の長さ</param>
	/// <param name="_height">Y方向の長さ</param>
	/// <param name="_depth">Z方向の長さ</param>
	BoxColliderComponent(EntityID _entity, float _width, float _height, float _depth);

	BoxColliderComponent(EntityID _entity, float _size);

	// X方向の長さ取得
	float GetWidth();
	// X方向の長さ変更
	void SetWidth(float _width);
	// Y方向の長さ取得
	float GetHeight();
	void SetHeight(float _heibht);
	// Z方向の長さ取得
	float GetDepth();
	// Z方向の長さ変更
	void SetDepth(float _depth);
private:
	float width{ 0.0f }, height{ 0.0f }, depth{ 0.0f };
};
