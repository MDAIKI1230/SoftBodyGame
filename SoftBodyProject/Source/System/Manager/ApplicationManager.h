#pragma once

#include "BackEnd.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include "ResourceManager.h"

class ApplicationManager
{
public:
	ApplicationManager(BackEnd&& _backEnd);
	int ApplicationMain();
private:
	std::unique_ptr<IRenderer> renderer;
	std::unique_ptr<InputSystem> inputSystem;
	std::unique_ptr<ISystem> system;
	std::unique_ptr<IGPUConnecter> gpuConnecter;
	std::unique_ptr<SceneManager> sceneManager;
	std::unique_ptr<TimeManager> timeManager;
	std::unique_ptr<ResourceManager> resourceManager;
};
