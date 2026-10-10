#include "TransformComponent.h"

#include "RigidBodyComponent.h"

#include "BoxColliderComponent.h"

#include "Color.h"

#include "DebugBox.h"

DebugBox::DebugBox(WorldStorage* _world, EntityID _entity) :
	MonoBehaviour{ _world,_entity }
{
	AddComponent<TransformComponent>();
	BoxColliderComponent* collider{ AddComponent<BoxColliderComponent>(30.0f) };
	Color c{ 1.0f, 1.0f, 1.0f };
}

// コンストラクタ
DebugBox::DebugBox(WorldStorage* _world, EntityID _entity, float _size) :
	MonoBehaviour{ _world,_entity }
{
	AddComponent<TransformComponent>();
	BoxColliderComponent* collider{ AddComponent<BoxColliderComponent>(_size) };
	Color c{ 1.0f, 1.0f, 1.0f };
}

// コンストラクタ
DebugBox::DebugBox(WorldStorage* _world, EntityID _entity, float _width, float _height, float _depth) :
	MonoBehaviour{ _world,_entity }
{
	AddComponent<TransformComponent>();
	BoxColliderComponent* collider{ AddComponent<BoxColliderComponent>(_width,_height,_depth) };
	Color c{ 1.0f, 1.0f, 1.0f };
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

void DebugBox::OnCollisionEnter(CollisionInfo _info)
{

}
void DebugBox::OnCollision(CollisionInfo _info)
{

}
void DebugBox::OnCollisionExit(CollisionInfo _info)
{
	
}
