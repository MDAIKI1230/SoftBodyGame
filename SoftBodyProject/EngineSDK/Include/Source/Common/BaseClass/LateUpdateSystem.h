#pragma once

#include "SystemBase.h"

#include "WorldStorage.h"
#include "EventManager.h"

class LateUpdateSystem :public SystemBase
{
public:
	virtual void LateUpdate(WorldStorage* _worldStorage, EventManager* _eventManager) = 0;

	// 仮想デストラクタ
	virtual ~LateUpdateSystem() = default;
};
