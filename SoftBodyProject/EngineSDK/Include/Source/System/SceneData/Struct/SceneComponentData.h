#pragma once

#include <cstddef>
#include <string>

struct SceneComponentData
{
public:
	// 保存形式に含めない、同じ型の中での配置順。
	std::size_t instanceIndex{ 0 };
	// 保存形式に含めない生成優先度。大きい値を先に生成する。
	int creationPriority{ 1 };
	virtual std::string GetName() = 0;
	virtual ~SceneComponentData() = default;
};