#pragma once

#include "ComponentBase.h"

#include "RopeID.h"

struct RopeComponent :public ComponentBase
{
public:
	// コンストラクタ
	RopeComponent(int _handle);
private:
	RopeID id;
};
