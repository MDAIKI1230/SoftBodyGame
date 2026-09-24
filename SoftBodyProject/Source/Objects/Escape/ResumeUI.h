#pragma once

#include "ObjectBase.h"

class ResumeUI :public ObjectBase
{
public:
	// コンストラクタ
	ResumeUI(WorldStorage* _world, EntityID _entityID);

	// ゲーム終了
	void Resume();
};