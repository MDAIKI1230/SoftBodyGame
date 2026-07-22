#pragma once

#include "ComponentBase.h"

#include "BodyID.h"

struct SoftBodyComponent :public ComponentBase
{
public:
	// コンストラクタ
	SoftBodyComponent(int _handle);
private:
	BodyID id;
};
