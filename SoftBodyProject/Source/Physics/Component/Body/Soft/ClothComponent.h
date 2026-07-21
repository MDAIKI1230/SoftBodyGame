#pragma once

#include "ComponentBase.h"

#include "ClothID.h"

struct ClothComponent :public ComponentBase
{
public:
	// コンストラクタ
	ClothComponent(int _handle);
private:
	ClothID id;
};
