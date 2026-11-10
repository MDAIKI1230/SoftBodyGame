#pragma once

#include "SceneBase.h"

class GameScene :public SceneBase
{
public:
	// コンストラクタ
	GameScene();
	// 初期化
	void Initialize() override;
};
