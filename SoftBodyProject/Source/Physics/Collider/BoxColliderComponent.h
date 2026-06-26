#pragma once

#include "ColliderComponent.h"

struct BoxColliderComponent :public ColliderComponent
{
public:
	// ---コンストラクタ---

	// デフォルトコンストラクタ
	BoxColliderComponent() = default;
	
	// 
private:
	int handle;
};
