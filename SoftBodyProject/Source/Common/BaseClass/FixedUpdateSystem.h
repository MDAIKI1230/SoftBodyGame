#pragma once

#include "SystemBase.h"

#include "WorldStorage.h"
#include "EventManager.h"

class FixedUpdateSystem : public SystemBase
{
public:
	virtual void FixedUpdate(WorldStorage* _worldStorage, EventManager* _eventManager) = 0;
	// 仮想デストラクタ
	virtual ~FixedUpdateSystem() = default;
};
