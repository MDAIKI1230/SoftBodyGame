#include "ServiceLocator.h"

#include "TransformComponent.h"

#include "SphereColliderComponent.h"

#include "Color.h"

#include "DebugSphere.h"

DebugSphere::DebugSphere(IWorld* world, int handle) :
	ObjectBase{ world,handle }
{
	AddComponent<TransformComponent>();
	SphereColliderComponent* collider{ AddComponent<SphereColliderComponent>(30.0f) };
	Color c{ 255, 255, 255 };
#ifdef _DEBUG
	collider->SetColor(c);
#endif // _DEBUG
}

// --- 更新系 ---

void DebugSphere::Update()
{
	TransformComponent* trans{ GetComponent<TransformComponent>() };

	if (ServiceLocator::GetInputManager()->GetKeyPress(KeyConstants::A))
	{
		trans->TranslateLocal(-Vector3::RIGHT);
	}
	if (ServiceLocator::GetInputManager()->GetKeyPress(KeyConstants::D))
	{
		trans->TranslateLocal(Vector3::RIGHT);
	}
	if (ServiceLocator::GetInputManager()->GetKeyPress(KeyConstants::W))
	{
		trans->TranslateLocal(Vector3::FORWARD);
	}
	if (ServiceLocator::GetInputManager()->GetKeyPress(KeyConstants::S))
	{
		trans->TranslateLocal(-Vector3::FORWARD);
	}
	if (ServiceLocator::GetInputManager()->GetKeyPress(KeyConstants::LSHIFT))
	{
		trans->TranslateLocal(-Vector3::UP);
	}
	if (ServiceLocator::GetInputManager()->GetKeyPress(KeyConstants::SPACE))
	{
		trans->TranslateLocal(Vector3::UP);
	}
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
