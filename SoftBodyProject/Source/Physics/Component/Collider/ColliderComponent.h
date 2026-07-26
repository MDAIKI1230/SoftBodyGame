#pragma once

#include "ColliderID.h"
#include "EntityID.h"

#ifdef _DEBUG
#include "Color.h"
#endif // DEBUG

struct ColliderComponent
{
public:
	// コンストラクタ
	ColliderComponent(ColliderID _colliderID) :
		colliderID{ _colliderID }
	{
	}

#ifdef _DEBUG
	// 色取得
	const Color& GetColor() const { return color; }
	// 色セット
	void SetColor(const Color& _color) { color = _color; }
#endif // DEBUG

	// 仮想デストラクタ
	virtual ~ColliderComponent() = default;
protected:
	ColliderID colliderID{};
	// デバッグの色
#ifdef _DEBUG
	Color color;
#endif // _DEBUG
};
