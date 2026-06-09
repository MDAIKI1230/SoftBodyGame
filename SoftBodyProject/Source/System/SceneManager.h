#pragma once

#include <memory>

#include "SceneBase.h"

class SceneManager
{
public:
	// コンストラクタ
	SceneManager();
	// 更新
	void Update();
	// 描画
	void Draw();
private:
	std::unique_ptr<SceneBase> currentScene;
	std::unique_ptr<SceneBase> nextScene;
};