#include "ResourceManager.h"

#include "SpriteRendererComponent.h"

#include "Check.h"

// コンストラクタ
Check::Check(WorldStorage* _world, EntityID _entityID) :
	MonoBehaviour{ _world,_entityID }
{
	ResourceManager::LoadTexture("Res/Texture/Check.png");

	AddComponent<SpriteRendererComponent>(ResourceManager::GetTexture("Check.png"));
}