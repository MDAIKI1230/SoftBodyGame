#pragma once

#include "SystemBase.h"

class RenderingSystem: public SystemBase
{
public:
	virtual void Draw(IWorld* world) = 0;
	// 仮想デストラクタ
	virtual ~RenderingSystem() = default;
};