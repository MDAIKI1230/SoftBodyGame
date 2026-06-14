#pragma once

#include "SystemBase.h"

class FixedUpdateSystem : public SystemBase
{
public:
	virtual void FixedUpdate(IWorld* world) = 0;
	// 仮想デストラクタ
	virtual ~FixedUpdateSystem() = default;
};