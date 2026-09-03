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
};
