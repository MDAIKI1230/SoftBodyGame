#pragma once

#include "ObjectBase.h"

class QuitUI :public ObjectBase
{
public:
	// コンストラクタ
	QuitUI(WorldStorage* _world, EntityID _entityID);

	// ゲーム終了
	void Quit();
};