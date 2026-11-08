#pragma once

#include "SystemBase.h"

#include "WorldStorage.h"
#include "EventManager.h"

class RenderingSystem: public SystemBase
{
public:
	virtual void Draw(WorldStorage* _worldStorage, EventManager* _eventManager) = 0;
	// 仮想デストラクタ
	virtual ~RenderingSystem() = default;
};
