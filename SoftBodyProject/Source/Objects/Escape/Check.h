#pragma once

#include "ObjectBase.h"

class Check :public ObjectBase
{
public:
	// コンストラクタ
	Check(WorldStorage* _world, EntityID _entityID);
};