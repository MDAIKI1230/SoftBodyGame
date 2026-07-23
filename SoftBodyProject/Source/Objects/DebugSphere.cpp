#include "ServiceLocator.h"

#include "TransformComponent.h"

#include "RigidBodyComponent.h"

#include "SphereColliderComponent.h"

#include "Color.h"

#include "DebugSphere.h"

DebugSphere::DebugSphere(WorldStorage* world, const EntityID& _entity) :
	ObjectBase{ world,_entity }
{
	AddComponent<TransformComponent>();
	SphereColliderComponent* collider{ AddComponent<SphereColliderComponent>(30.0f) };
	Color c{ 255, 255, 255 };
	RigidBodyComponent* body{ AddComponent<RigidBodyComponent>() };
	// body->SetIsGravity(false);
#ifdef _DEBUG
	collider->SetColor(c);
#endif // _DEBUG
}

// --- 更新系 ---

void DebugSphere::Update()
{
	
}
void DebugSphere::FixedUpdate()
{

}

// --- 衝突系 ---

void DebugSphere::OnCollisionEnter()
{
	Color c{ 255,0,0 };
#ifdef _DEBUG
	GetComponent<SphereColliderComponent>()->SetColor(c);
#endif // DEBUG
}
void DebugSphere::OnCollision()
{

}
void DebugSphere::OnCollisionExit()
{
	Color c{ 255,255,255 };
#ifdef _DEBUG
	GetComponent<SphereColliderComponent>()->SetColor(c);
#endif // DEBUG
}
