#pragma once

#include "SceneBase.h"

#include "InputActionContext.h"

class TitleScene :public SceneBase
{
public:
	// コンストラクタ
	TitleScene();
	// 初期化
	void Initialize() override;

private:
	void ToGameScene(InputActionContext _input);
};
