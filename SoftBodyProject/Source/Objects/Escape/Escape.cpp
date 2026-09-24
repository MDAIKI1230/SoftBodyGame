#include "ResourceManager.h"

#include "SpriteRendererComponent.h"

#include "Escape.h"

// コンストラクタ
Escape::Escape(WorldStorage* _world, EntityID _entityID) :
	ObjectBase{ _world,_entityID }
{
	ResourceManager::LoadTexture("Res/Texture/Escape.png");

	AddComponent<SpriteRendererComponent>(ResourceManager::GetTexture("Escape.png"));
}

// --- 更新系 ---

// 更新処理
void Escape::Update()
{

}
// 物理更新処理
void Escape::FixedUpdate()
{

}

// エスケープの選択肢追加
void Escape::AddSlecte()
{

}

void Escape::SlectedMouse(InputActionContext _input)
{

}
void Escape::SlectedButton(InputActionContext _input)
{

}
void Escape::Action(InputActionContext _input)
{

}