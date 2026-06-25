#pragma once

#include "SystemBase.h"

class FixedUpdateSystem : public SystemBase
{
public:
	virtual void FixedUpdate(WorldStorage* world) = 0;
	// 仮想デストラクタ
	virtual ~FixedUpdateSystem() = default;
};
