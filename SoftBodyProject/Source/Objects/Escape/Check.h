#pragma once

#include "MonoBehaviour.h"

class Check :public MonoBehaviour
{
public:
	// コンストラクタ
	Check(WorldStorage* _world, EntityID _entityID);
};