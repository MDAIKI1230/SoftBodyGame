#pragma once

#include <memory>
#include <document.h>
#include <unordered_map>

#include "MDMath.h"

#include "SceneComponentData.h"

namespace JsonLoadHelperFunc
{
	using ComponentCreateDataFunc = std::unique_ptr<SceneComponentData>(*)(const rapidjson::Value&);
	// --- コンポ―ネントデータ作成関数 ---

	std::unique_ptr<SceneComponentData> CreateTransformData(const rapidjson::Value& _value);
	std::unique_ptr<SceneComponentData> CreateBoxColliderData(const rapidjson::Value& _value);
	std::unique_ptr<SceneComponentData> CreateSphereColliderData(const rapidjson::Value& _value);
	std::unique_ptr<SceneComponentData> CreateRendererData(const rapidjson::Value& _value);

	// ベクトル取得関数
	bool GetVector3(const rapidjson::Value& _value,Vector3& _output);

	// コンポーネント名と関数MAP
	static std::unordered_map<std::string, ComponentCreateDataFunc> CreateFuncs
	{
		{"Transform",      &CreateTransformData},
		{"BoxCollider",    &CreateBoxColliderData},
		{"SphereCollider", &CreateSphereColliderData},
		{"Renderer",       &CreateRendererData},
	};
}
