#pragma once

#include "ObjectBase.h"

class TitleUI :public ObjectBase
{
public:
	// コンストラクタ
	TitleUI(WorldStorage* _world, EntityID _entityID);

	// ゲーム終了
	void ToTitleScene();
};