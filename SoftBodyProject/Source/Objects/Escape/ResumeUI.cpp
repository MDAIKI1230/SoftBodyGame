#include "ResourceManager.h"
#include "GameManager.h"

#include "SpriteRendererComponent.h"

#include "ResumeUI.h"

// コンストラクタ
ResumeUI::ResumeUI(WorldStorage* _world, EntityID _entityID) :
	EscapePart{ _world,_entityID }
{
	ResourceManager::LoadTexture("Res/Texture/Resume.png");

	AddComponent<SpriteRendererComponent>(ResourceManager::GetTexture("Resume.png"));
}

// 元のシーンに戻る
void ResumeUI::Action()
{
	GameManager::StartScene();
}