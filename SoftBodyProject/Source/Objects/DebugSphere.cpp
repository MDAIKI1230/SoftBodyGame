#include "ServiceLocator.h"

#include "TransformComponent.h"

#include "RigidBodyComponent.h"

#include "SphereColliderComponent.h"

#include "Color.h"

#include "DebugSphere.h"

DebugSphere::DebugSphere(WorldStorage* world, EntityID _entity) :
	ObjectBase{ world,_entity }
{
	AddComponent<TransformComponent>();
	SphereColliderComponent* collider{ AddComponent<SphereColliderComponent>(30.0f) };
	Color c{ 1.0f, 1.0f, 1.0f };
	RigidBodyComponent* body{ AddComponent<RigidBodyComponent>() };
	// body->SetIsGravity(false);
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
	Color c{ 1.0f,0,0 };
}
void DebugSphere::OnCollision()
{

}
void DebugSphere::OnCollisionExit()
{
	Color c{ 1.0f,1.0f,1.0f };
}
