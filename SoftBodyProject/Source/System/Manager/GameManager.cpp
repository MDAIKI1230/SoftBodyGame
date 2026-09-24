#include "GameManager.h"

void GameManager::StartSceneImpl()
{
	phase = CurrentPhase::SCENE;
}
void GameManager::StartEscapeImpl()
{
	phase = CurrentPhase::ESCAPE;
}

bool GameManager::IsSceneImpl()
{
	return phase == CurrentPhase::SCENE;
}
bool GameManager::IsEscapeImpl()
{
	return phase == CurrentPhase::ESCAPE;
}