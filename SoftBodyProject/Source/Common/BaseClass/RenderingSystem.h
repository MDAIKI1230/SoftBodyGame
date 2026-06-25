#pragma once

#include "SystemBase.h"

class RenderingSystem: public SystemBase
{
public:
	virtual void Draw(WorldStorage* world) = 0;
	// 仮想デストラクタ
	virtual ~RenderingSystem() = default;
};
