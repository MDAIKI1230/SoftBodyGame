#pragma once

#include "MDJson.h"
#include "SceneFileData.h"
#include "SceneObjectData.h"

namespace SceneDataLoadHelperFunc
{
	// シーンロード
	bool LoadScene(const MDJson::Value& _value, SceneFileData& _output);
	// コンポーネント以外のオブジェクトすべてのロード
	bool LoadObjects(const MDJson::Value& _value, std::unordered_map<std::uint64_t, SceneObjectData>& _output);
	// コンポーネント以外のオブジェクトのロード
	bool LoadObjectHierarchy(const MDJson::Value& _value, std::unordered_map<std::uint64_t, SceneObjectData>& _output, const std::uint64_t& _parent);
	// コンポーネントのロード
	bool LoadComponents(const MDJson::Value& _value, TransformComponentData& _transform, std::unordered_map<std::string, std::vector<std::unique_ptr<SceneComponentData>>>& _output, std::vector<SceneComponentData*>& _creationOrder);
}