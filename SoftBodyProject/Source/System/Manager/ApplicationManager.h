#pragma once

#include "IRenderer.h"
#include "IInput.h"
#include "ISystem.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include "WorldStorage.h"
#include "SystemManager.h"

class ApplicationManager
{
public:
	ApplicationManager();
	int ApplicationMain();
private:
	std::unique_ptr<IRenderer> renderer;
	std::unique_ptr<IInput> input;
	std::unique_ptr<ISystem> system;
	std::unique_ptr<SceneManager> sceneManager;
	std::unique_ptr<TimeManager> timeManager;
	std::unique_ptr<WorldStorage> worldStorage;
	std::unique_ptr<SystemManager> systemManager;
};
