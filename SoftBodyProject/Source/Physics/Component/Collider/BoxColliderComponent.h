#pragma once

#include "ColliderComponent.h"

#include "EntityID.h"

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
	float GetWidth() const;
	// X方向の長さ変更
	void SetWidth(float _width);
	// Y方向の長さ取得
	float GetHeight() const;
	void SetHeight(float _heibht);
	// Z方向の長さ取得
	float GetDepth() const;
	// Z方向の長さ変更
	void SetDepth(float _depth);

	// オフセット位置取得
	Vector3 GetOffsetPosition();
	// オフセット位置変更
	void SetOffsetPosition(const Vector3& _offset);
	// オフセット回転取得
	Quaternion GetOffsetRotation();
	// オフセット回転変更
	void SetOffsetRotation(const Quaternion& _offset);
};
