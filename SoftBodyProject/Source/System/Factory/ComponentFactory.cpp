#include "TransformComponent.h"
#include "BoxColliderComponent.h"
#include "SphereColliderComponent.h"
#include "RendererComponent.h"
#include "ResourceManager.h"

#include <filesystem>

#include "TransformComponentData.h"
#include "BoxColliderComponentData.h"
#include "SphereColliderComponentData.h"
#include "RendererComponentData.h"

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
	const Vector3& euler{ transformData->ratationDeg };
	trans->SetRotation(Quaternion::Euler(
		MDMath::DegToRad(euler.x),
		MDMath::DegToRad(euler.y),
		MDMath::DegToRad(euler.z)));
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
	box->SetOffsetPosition(boxData->offsetPosition);
	const Vector3& euler{ boxData->offsetRotationDeg };
	box->SetOffsetRotation(Quaternion::Euler(
		MDMath::DegToRad(euler.x),
		MDMath::DegToRad(euler.y),
		MDMath::DegToRad(euler.z)));
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
	sphere->SetOffsetPosition(sphereData->offsetPosition);
	const Vector3& euler{ sphereData->offsetRotationDeg };
	sphere->SetOffsetRotation(Quaternion::Euler(
		MDMath::DegToRad(euler.x),
		MDMath::DegToRad(euler.y),
		MDMath::DegToRad(euler.z)));
}

void ComponentFactory::CreateRendererComponent(ObjectBase* _obj, SceneComponentData* _data)
{
	RendererComponentData* rendererData{ static_cast<RendererComponentData*>(_data) };

	if (rendererData->modelPath.empty() || _obj->GetComponent<RendererComponent>() != nullptr)
	{
		return;
	}

	const std::filesystem::path modelPath{ rendererData->modelPath };
	ModelHandle model{ ResourceManager::GetModel(modelPath.filename()) };

	if (!model.IsValid())
	{
		if (!ResourceManager::LoadModel(modelPath))
		{
			return;
		}

		model = ResourceManager::GetModel(modelPath.filename());
	}

	if (model.IsValid())
	{
		_obj->AddComponent<RendererComponent>(model);
	}
}

std::unordered_map<std::string, ComponentFactory::CreateComponentFunc> ComponentFactory::CreateFuncs
{
	{ "Transform",      &ComponentFactory::CreateTransformComponent },
	{ "BoxCollider",    &ComponentFactory::CreateBoxColliderComponent },
	{ "SphereCollider", &ComponentFactory::CreateSphereColliderComponent },
	{ "Renderer",       &ComponentFactory::CreateRendererComponent }
};
