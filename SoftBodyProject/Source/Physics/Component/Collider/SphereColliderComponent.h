#pragma once

#include "ColliderComponent.h"

#include "EntityID.h"

struct SphereColliderComponent :public ColliderComponent
{
public:
	// --- コンストラクタ ---

	// コンストラクタ
	SphereColliderComponent(EntityID _entity);
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_r">半径</param>
	SphereColliderComponent(EntityID _entity, float _r);

	// --- 半径 ---

	// 半径セット
	void SetRadius(float _r);
	// 半径取得
	float GetRadius() const;

	// オフセット位置取得
	Vector3 GetOffsetPosition();
	// オフセット位置変更
	void SetOffsetPosition(const Vector3& _offset);
	// オフセット回転取得
	Quaternion GetOffsetRotation();
	// オフセット回転変更
	void SetOffsetRotation(const Quaternion& _offset);
};
