#pragma once

#include "BackEnd.h"
#include "SceneManager.h"
#include "TimeManager.h"

class ApplicationManager
{
public:
	ApplicationManager(BackEnd&& _backEnd);
	int ApplicationMain();
private:
	std::unique_ptr<IRenderer> renderer;
	std::unique_ptr<IInput> input;
	std::unique_ptr<ISystem> system;
	std::unique_ptr<SceneManager> sceneManager;
	std::unique_ptr<TimeManager> timeManager;
};
