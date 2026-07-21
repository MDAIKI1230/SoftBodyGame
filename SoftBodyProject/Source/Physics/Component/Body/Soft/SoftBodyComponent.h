#pragma once

#include "ComponentBase.h"

#include "SoftBodyID.h"

struct SoftBodyComponent :public ComponentBase
{
public:
	// コンストラクタ
	SoftBodyComponent(int _handle);
private:
	SoftBodyID id;
};
