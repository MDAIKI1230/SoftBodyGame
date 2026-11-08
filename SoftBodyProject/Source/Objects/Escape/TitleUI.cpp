#include "ResourceManager.h"

#include "SpriteRendererComponent.h"

#include "ApplicationRequest.h"

#include "TitleUI.h"

// コンストラクタ
TitleUI::TitleUI(WorldStorage* _world, EntityID _entityID) :
	EscapePart{ _world,_entityID }
{
	ResourceManager::LoadTexture("Res/Texture/Title.png");

	AddComponent<SpriteRendererComponent>(ResourceManager::GetTexture("Title.png"));
}

// タイトルに戻る
void TitleUI::Action()
{
	ApplicationRequest::SceneRequest(SceneRequestType::TITLE);
}