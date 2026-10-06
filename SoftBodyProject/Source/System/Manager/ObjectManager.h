#pragma once

#include <vector>
#include <memory>

#include "MonoBehaviour.h"

class ObjectManager
{
public:
	// --- 追加・削除系 ---

	// 追加
	void Add(std::unique_ptr<MonoBehaviour> _object);

	// --- 更新系 ---

	void Update();
	void FixedUpdate();

	// オブジェクト取得
	MonoBehaviour* Get(EntityID _index);

	// EntityHandle取得
	EntityID GenerateNewID()
	{
		return EntityID{ static_cast<EntityID::Index>(objects.size()),1 };
	}
private:
	std::vector<std::unique_ptr<MonoBehaviour>> objects{};
};
