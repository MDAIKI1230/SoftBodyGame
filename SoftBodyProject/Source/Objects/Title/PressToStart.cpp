#include "ResourceManager.h"
#include "TimeManager.h"

#include "SpriteRendererComponent.h"
#include "TransformComponent.h"

#include "PressToStart.h"

// コンストラクタ
PressToStart::PressToStart(WorldStorage* _world, EntityID _entityID) :
	ObjectBase{ _world,_entityID }
{
	AddComponent<SpriteRendererComponent>(ResourceManager::GetTexture("PressToStart.png"));
	GetComponent<TransformComponent>()->SetPosition(initialPos);
}

// --- 更新系 ---

// 更新処理
void PressToStart::Update()
{
	Vector3 position{ GetComponent<TransformComponent>()->GetPosition() };

	position.y = initialPos.y + sinf(time += TimeManager::GetDeltaTime() * 2.0f) * 20.0f;

	GetComponent<TransformComponent>()->SetPosition(position);
}