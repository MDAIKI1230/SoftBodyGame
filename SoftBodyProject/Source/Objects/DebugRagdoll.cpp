#include "ResourceManager.h"

#include "RagdollComponent.h"
#include "RendererComponent.h"
#include "TransformComponent.h"

#include "DebugRagdoll.h"

// コンストラクタ
DebugRagdoll::DebugRagdoll(WorldStorage* _world, EntityID _entity) :
	ObjectBase{ _world,_entity }
{
	AddComponent<TransformComponent>();//->SetRotation(Quaternion::Euler(0.0f, 0.0f, MathConstants::PI_FLT / 2.0f));
	RendererComponent* renderer{ AddComponent<RendererComponent>(ResourceManager::GetModel("M_001_player_095_01_no_sword_walk.mv1")) };
	AddComponent<RagdollComponent>(*renderer,"Res/Data/Ragdoll/M_001_player_095_01_no_sword_tpose.json");
}
// --- 更新系 ---

void DebugRagdoll::Update()
{

}
void DebugRagdoll::FixedUpdate()
{

}
// --- 衝突系 ---

void DebugRagdoll::OnCollisionEnter()
{

}
void DebugRagdoll::OnCollision()
{

}
void DebugRagdoll::OnCollisionExit()
{

}