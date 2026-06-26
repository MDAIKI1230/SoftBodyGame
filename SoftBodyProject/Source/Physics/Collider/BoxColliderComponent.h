#pragma once

#include "ColliderComponent.h"

struct BoxColliderComponent :public ColliderComponent
{
public:
	// ---コンストラクタ---

	// デフォルトコンストラクタ
	BoxColliderComponent(int _handle) :
		handle{ _handle }
	{
	}
	
	BoxColliderComponent(float width, float height, float depth);
private:
	int handle;
};
