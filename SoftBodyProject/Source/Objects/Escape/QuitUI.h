#pragma once

#include "EscapePart.h"

class QuitUI :public EscapePart
{
public:
	// コンストラクタ
	QuitUI(WorldStorage* _world, EntityID _entityID);

	// ゲーム終了
	void Action() override;
};