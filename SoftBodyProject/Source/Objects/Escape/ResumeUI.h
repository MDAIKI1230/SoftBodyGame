#pragma once

#include "EscapePart.h"

class ResumeUI :public EscapePart
{
public:
	// コンストラクタ
	ResumeUI(WorldStorage* _world, EntityID _entityID);

	// 元のシーンに戻る
	void Action() override;
};