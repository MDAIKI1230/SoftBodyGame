#pragma once

#include "SystemBase.h"

#include "WorldStorage.h"
#include "EventManager.h"

class UpdateSystem :public SystemBase
{
public:
	virtual void Update(WorldStorage* _worldStorage, EventManager* _eventManager) = 0;
	// 仮想デストラクタ
	virtual ~UpdateSystem() = default;
};
