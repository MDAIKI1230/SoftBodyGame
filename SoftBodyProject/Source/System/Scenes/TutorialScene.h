#pragma once

#include "SceneBase.h"

class TutorialScene :public SceneBase
{
public:
	// コンストラクタ
	TutorialScene() = default;
	// 初期化
	void Initialize() override;
};
