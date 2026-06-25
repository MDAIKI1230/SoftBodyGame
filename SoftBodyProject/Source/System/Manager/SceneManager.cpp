#include "Scenes/DebugScene.h"

#include "SceneManager.h"

SceneManager::SceneManager(WorldStorage* _worldStorage, SystemManager* _systemManager) :
	worldStorage{ _worldStorage },
	systemManager{ _systemManager }
{
	currentScene = std::make_unique<DebugScene>(worldStorage, systemManager);
}

void SceneManager::Update()
{
	currentScene->Execute();
}

// 描画
void SceneManager::Draw()
{
	currentScene->Draw();
}
