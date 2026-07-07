#include "TransformComponent.h"
#include "BoxColliderComponent.h"
#include "SphereColliderComponent.h"

#include "TransformComponentData.h"
#include "BoxColliderComponentData.h"
#include "SphereColliderComponentData.h"

#include "ComponentFactory.h"

void ComponentFactory::CreateTransformComponent(ObjectBase* _obj, SceneComponentData* _data)
{
	// データをキャスト
	TransformComponentData* transformData{ static_cast<TransformComponentData*>(_data) };

	// 取得
	TransformComponent* trans{ _obj->GetComponent<TransformComponent>() };
	// ないならから新たに追加
	if (trans == nullptr)
	{
		trans = _obj->AddComponent<TransformComponent>();
	}

	// 値を設定
	trans->SetPosition(transformData->position);
	Vector3 euler{ transformData->ratationDeg };
	trans->SetRotation(Quaternion::Euler(euler.x, euler.y, euler.z));
	trans->SetScale(transformData->scale);
}

void ComponentFactory::CreateBoxColliderComponent(ObjectBase* _obj, SceneComponentData* _data)
{
	// データをキャスト
	BoxColliderComponentData* boxData{ static_cast<BoxColliderComponentData*>(_data) };

	// 取得
	BoxColliderComponent* box{ _obj->GetComponent<BoxColliderComponent>() };
	// ないならから新たに追加
	if (box == nullptr)
	{
		box = _obj->AddComponent<BoxColliderComponent>();
	}

	// 値を設定
	box->SetWidth(boxData->size.x);
	box->SetHeight(boxData->size.y);
	box->SetDepth(boxData->size.z);
}

void ComponentFactory::CreateSphereColliderComponent(ObjectBase* _obj, SceneComponentData* _data)
{
	// データをキャスト
	SphereColliderComponentData* sphereData{ static_cast<SphereColliderComponentData*>(_data) };

	// 取得
	SphereColliderComponent* sphere{ _obj->GetComponent<SphereColliderComponent>() };
	// ないならから新たに追加
	if (sphere == nullptr)
	{
		sphere = _obj->AddComponent<SphereColliderComponent>();
	}

	// 値を設定
	sphere->SetRadius(sphereData->radius);
}

std::unordered_map<std::string, ComponentFactory::CreateComponentFunc> ComponentFactory::CreateFuncs
{
	{ "Transform",      &ComponentFactory::CreateTransformComponent },
	{ "BoxCollider",    &ComponentFactory::CreateBoxColliderComponent },
	{ "SphereCollider", &ComponentFactory::CreateSphereColliderComponent }
};
