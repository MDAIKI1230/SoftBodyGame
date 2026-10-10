#include "ResourceManager.h"

#include "RagdollComponent.h"
#include "RendererComponent.h"
#include "TransformComponent.h"

#include "DebugRagdoll.h"

// コンストラクタ
DebugRagdoll::DebugRagdoll(WorldStorage* _world, EntityID _entity) :
	MonoBehaviour{ _world,_entity }
{
	AddComponent<TransformComponent>();
	RendererComponent* renderer{ AddComponent<RendererComponent>(ResourceManager::GetModel("M_001_player_095_01.mv1")) };
	AddComponent<RagdollComponent>(*renderer,"Res/Data/Ragdoll/M_001_player_095_01.json");
}
// --- 更新系 ---

void DebugRagdoll::Update()
{

}
void DebugRagdoll::FixedUpdate()
{

}
// --- 衝突系 ---

void DebugRagdoll::OnCollisionEnter(CollisionInfo _info)
{

}
void DebugRagdoll::OnCollision(CollisionInfo _info)
{

}
void DebugRagdoll::OnCollisionExit(CollisionInfo _info)
{

}