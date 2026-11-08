#pragma once

#include "ColliderComponent.h"
#include "EntityID.h"

struct CapsuleColliderComponent :public ColliderComponent
{
public:
	// ---コンストラクタ---

	// コンストラクタ
	CapsuleColliderComponent(EntityID _entity);

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_handle">ID</param>
	/// <param name="_width">X方向の長さ</param>
	/// <param name="_height">Y方向の長さ</param>
	/// <param name="_depth">Z方向の長さ</param>
	CapsuleColliderComponent(EntityID _entity, float _height, float _radius);

	// 高さ取得
	float GetHeight() const;
	// 高さ変更
	void SetHeight(float _heibht);
	// 半径取得
	float GetRadius() const;
	// 半径変更
	void SetRadius(float _radius);

	// オフセット位置取得
	Vector3 GetOffsetPosition();
	// オフセット位置変更
	void SetOffsetPosition(const Vector3& _offset);
};