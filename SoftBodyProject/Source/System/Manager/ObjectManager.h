#pragma once

#include <vector>
#include <memory>

#include "ObjectBase.h"

class ObjectManager
{
public:
	// --- 追加・削除系 ---

	// 追加
	void Add(std::unique_ptr<ObjectBase> _object);

	// --- 更新系 ---

	void Update();
	void FixedUpdate();

	// オブジェクト取得
	ObjectBase* Get(int _index);
private:
	std::vector<std::unique_ptr<ObjectBase>> objects;
};
