#pragma once

#include "ComponentBase.h"

#include "BodyID.h"

struct RopeComponent :public ComponentBase
{
public:
	// コンストラクタ
	RopeComponent(int _handle);
private:
	BodyID id;
};
