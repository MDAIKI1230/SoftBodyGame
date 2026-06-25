#pragma once

#include <memory>

#include "SceneBase.h"

class SceneManager
{
public:
	// コンストラクタ
	SceneManager(WorldStorage* _worldStorage, SystemManager* _systemManager);
	// 更新
	void Update();
	// 描画
	void Draw();
private:
	std::unique_ptr<SceneBase> currentScene;
	std::unique_ptr<SceneBase> nextScene;

	// ワールドストレージ
	WorldStorage* worldStorage;
	// システムマネージャー
	SystemManager* systemManager;
};
