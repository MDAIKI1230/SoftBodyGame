#pragma once

#include "ObjectBase.h"

class EscapePart :public ObjectBase
{
public:
	// コンストラクタ
	EscapePart(WorldStorage* _world, EntityID _entityID) :
		ObjectBase{ _world ,_entityID }
	{
	}

	// 各種アクション
	virtual void Action() = 0;
};