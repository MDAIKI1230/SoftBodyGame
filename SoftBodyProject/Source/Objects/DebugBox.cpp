#include "ServiceLocator.h"

#include "TransformComponent.h"

#include "RigidBodyComponent.h"

#include "BoxColliderComponent.h"

#include "Color.h"

#include "DebugBox.h"

DebugBox::DebugBox(WorldStorage* _world, const EntityID& _entity) :
	ObjectBase{ _world,_entity }
{
	AddComponent<TransformComponent>();
	BoxColliderComponent* collider{ AddComponent<BoxColliderComponent>(30.0f) };
	Color c{ 255, 255, 255 };
#ifdef _DEBUG
	collider->SetColor(c);
#endif // _DEBUG
}

// コンストラクタ
DebugBox::DebugBox(WorldStorage* _world, const EntityID& _entity, float _size) :
	ObjectBase{ _world,_entity }
{
	AddComponent<TransformComponent>();
	BoxColliderComponent* collider{ AddComponent<BoxColliderComponent>(_size) };
	Color c{ 255, 255, 255 };
#ifdef _DEBUG
	collider->SetColor(c);
#endif // _DEBUG
}

// コンストラクタ
DebugBox::DebugBox(WorldStorage* _world, const EntityID& _entity, float _width, float _height, float _depth) :
	ObjectBase{ _world,_entity }
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
	/*Quaternion rot{ Quaternion::AngleAxis(3.14159265 / 90 * ServiceLocator::GetTimeManager()->GetDeltaTime() , Vector3::UP)};
	GetComponent<TransformComponent>()->Rotate(rot);*/

	//Vector3 rotVec{ rot.Rotate(vec) };
	//Vector3 matVec{ MatGenerateFunc::Rotate(rot) * vec };

	//if (rotVec != matVec)
	//{
	//	rotVec + matVec;
	//}
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
