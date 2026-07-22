#pragma once

#include "ComponentBase.h"

#include "BodyID.h"

struct ClothComponent :public ComponentBase
{
public:
	// コンストラクタ
	ClothComponent(int _handle);
private:
	BodyID id;
};
