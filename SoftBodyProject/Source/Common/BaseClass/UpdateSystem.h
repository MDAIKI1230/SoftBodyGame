#pragma once

#include "SystemBase.h"

class UpdateSystem :public SystemBase
{
public:
	virtual void Update(WorldStorage* world) = 0;
	// 仮想デストラクタ
	virtual ~UpdateSystem() = default;
};
