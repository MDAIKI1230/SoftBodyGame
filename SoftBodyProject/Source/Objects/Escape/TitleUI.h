#pragma once

#include "EscapePart.h"

class TitleUI :public EscapePart
{
public:
	// コンストラクタ
	TitleUI(WorldStorage* _world, EntityID _entityID);

	// タイトルに戻る
	void Action() override;
};