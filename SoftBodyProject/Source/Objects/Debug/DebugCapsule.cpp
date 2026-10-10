#include "TransformComponent.h"

#include "RigidBodyComponent.h"

#include "CapsuleColliderComponent.h"

#include "DebugCapsule.h"

// コンストラクタ
DebugCapsule::DebugCapsule(WorldStorage* world, EntityID _entity):
	MonoBehaviour{ world,_entity }
{
	AddComponent<TransformComponent>();
	CapsuleColliderComponent* collider{ AddComponent<CapsuleColliderComponent>(20.0f,5.0f) };

	RigidBodyComponent* body{ AddComponent<RigidBodyComponent>() };
}

// --- 更新系 ---

void DebugCapsule::Update()
{

}
void DebugCapsule::FixedUpdate()
{

}

// --- 衝突系 ---

void DebugCapsule::OnCollisionEnter(CollisionInfo _info)
{

}
void DebugCapsule::OnCollision(CollisionInfo _info)
{

}
void DebugCapsule::OnCollisionExit(CollisionInfo _info)
{

}
