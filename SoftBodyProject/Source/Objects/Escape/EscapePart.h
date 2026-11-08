#pragma once

#include "MonoBehaviour.h"

class EscapePart :public MonoBehaviour
{
public:
	// コンストラクタ
	EscapePart(WorldStorage* _world, EntityID _entityID) :
		MonoBehaviour{ _world ,_entityID }
	{
	}

	// 各種アクション
	virtual void Action() = 0;
};