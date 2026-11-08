#pragma once

#include "SceneManager.h"

class GameApplication
{
public:
	GameApplication();

	int Run();
private:
	std::unique_ptr<SceneManager> sceneManager;
};