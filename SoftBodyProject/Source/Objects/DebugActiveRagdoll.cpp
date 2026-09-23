#include "ResourceManager.h"

#include "ActiveRagdollComponent.h"
#include "AnimationComponent.h"
#include "RendererComponent.h"
#include "TransformComponent.h"

#include "DebugActiveRagdoll.h"

// コンストラクタ
DebugActiveRagdoll::DebugActiveRagdoll(WorldStorage* _world, EntityID _entity) :
	ObjectBase{ _world,_entity }
{
	AddComponent<TransformComponent>();
	RendererComponent* renderer{ AddComponent<RendererComponent>(ResourceManager::GetModel("M_001_player_095_01_no_sword_walk.mv1")) };
	AddComponent<ActiveRagdollComponent>(*renderer, "Res/Data/Ragdoll/Active/M_001_player_095_01_no_sword.json");
	AnimationComponent* anim{ AddComponent<AnimationComponent>(renderer) };
	anim->SetAnimationName("Walk");
	//anim->Play();
	anim->SetLoop(true);
}
// --- 更新系 ---

void DebugActiveRagdoll::Update()
{

}
void DebugActiveRagdoll::FixedUpdate()
{

}
// --- 衝突系 ---

void DebugActiveRagdoll::OnCollisionEnter()
{

}
void DebugActiveRagdoll::OnCollision()
{

}
void DebugActiveRagdoll::OnCollisionExit()
{

}