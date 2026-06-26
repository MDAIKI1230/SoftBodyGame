#include "ServiceLocator.h"

#include "TransformComponent.h"

#include "RigidBodyComponent.h"

#include "BoxColliderComponent.h"

#include "Color.h"

#include "DebugBox.h"

DebugBox::DebugBox(WorldStorage* _world, int _handle) :
	ObjectBase{ _world,_handle }
{
	AddComponent<TransformComponent>();
	BoxColliderComponent* collider{ AddComponent<BoxColliderComponent>(30.0f) };
	Color c{ 255, 255, 255 };
#ifdef _DEBUG
	collider->SetColor(c);
#endif // _DEBUG
}

// コンストラクタ
DebugBox::DebugBox(WorldStorage* _world, int _handle, float _size) :
	ObjectBase{ _world,_handle }
{
	AddComponent<TransformComponent>();
	BoxColliderComponent* collider{ AddComponent<BoxColliderComponent>(_size) };
	Color c{ 255, 255, 255 };
#ifdef _DEBUG
	collider->SetColor(c);
#endif // _DEBUG
}

// コンストラクタ
DebugBox::DebugBox(WorldStorage* _world, int _handle, float _width, float _height, float _depth) :
	ObjectBase{ _world,_handle }
{
	AddComponent<TransformComponent>();
	BoxColliderComponent* collider{ AddComponent<BoxColliderComponent>(_width,_height,_depth) };
	Color c{ 255, 255, 255 };
#ifdef _DEBUG
	collider->SetColor(c);
#endif // _DEBUG
}

// --- 更新系 ---

void DebugBox::Update()
{
	TransformComponent* trans{ GetComponent<TransformComponent>() };
	Quaternion rot{ Quaternion::AngleAxis(-0.91f , Vector3::UP) };

	trans->RotateEuler(Vector3(0, 1, 0));
}
void DebugBox::FixedUpdate()
{

}

// --- 衝突系 ---

void DebugBox::OnCollisionEnter()
{
	Color c{ 255,0,0 };
#ifdef _DEBUG
	GetComponent<BoxColliderComponent>()->SetColor(c);
#endif // DEBUG
}
void DebugBox::OnCollision()
{

}
void DebugBox::OnCollisionExit()
{
	Color c{ 255,255,255 };
#ifdef _DEBUG
	GetComponent<BoxColliderComponent>()->SetColor(c);
#endif // DEBUG
}
