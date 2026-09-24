#include "ResourceManager.h"

#include "SpriteRendererComponent.h"

#include "ApplicationRequest.h"

#include "QuitUI.h"

// コンストラクタ
QuitUI::QuitUI(WorldStorage* _world, EntityID _entityID):
	EscapePart{_world,_entityID}
{
	ResourceManager::LoadTexture("Res/Texture/Quit.png");

	AddComponent<SpriteRendererComponent>(ResourceManager::GetTexture("Quit.png"));
}

// ゲーム終了
void QuitUI::Action()
{
	ApplicationRequest::ExitRequest();
}