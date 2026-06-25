#include "Scenes/DebugScene.h"

#include "SceneManager.h"

SceneManager::SceneManager()
{
	currentScene = std::make_unique<DebugScene>();
}

void SceneManager::Update()
{
	currentScene->Execute();
}

void SceneManager::Render()
{
	currentScene->Render();
}
