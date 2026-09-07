#pragma once

#include "BackEnd.h"
#include "SceneManager.h"

class ApplicationManager
{
public:
	ApplicationManager(BackEnd&& _backEnd);
	int ApplicationMain();
private:
	std::unique_ptr<IRenderer> renderer;
	std::unique_ptr<ISystem> system;
	std::unique_ptr<IGPUConnecter> gpuConnecter;
	std::unique_ptr<SceneManager> sceneManager;
};
