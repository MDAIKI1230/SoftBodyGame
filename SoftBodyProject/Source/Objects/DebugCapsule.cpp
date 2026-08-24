#include "TransformComponent.h"

#include "RigidBodyComponent.h"

#include "CapsuleColliderComponent.h"

#include "DebugCapsule.h"

// コンストラクタ
DebugCapsule::DebugCapsule(WorldStorage* world, EntityID _entity):
	ObjectBase{ world,_entity }
{
	AddComponent<TransformComponent>();
	CapsuleColliderComponent* collider{ AddComponent<CapsuleColliderComponent>(20.0f,5.0f) };
	Color c{ 1.0f, 1.0f, 1.0f };
	RigidBodyComponent* body{ AddComponent<RigidBodyComponent>() };
	// body->SetIsGravity(false);
#ifdef _DEBUG
	collider->SetColor(c);
#endif // _DEBUG
}

// --- 更新系 ---

void DebugCapsule::Update()
{

}
void DebugCapsule::FixedUpdate()
{

}

// --- 衝突系 ---

void DebugCapsule::OnCollisionEnter()
{
	Color c{ 1.0f,0,0 };
#ifdef _DEBUG
	GetComponent<CapsuleColliderComponent>()->SetColor(c);
#endif // DEBUG
}
void DebugCapsule::OnCollision()
{

}
void DebugCapsule::OnCollisionExit()
{
	Color c{ 1.0f,1.0f,1.0f };
#ifdef _DEBUG
	GetComponent<CapsuleColliderComponent>()->SetColor(c);
#endif // DEBUG
}
