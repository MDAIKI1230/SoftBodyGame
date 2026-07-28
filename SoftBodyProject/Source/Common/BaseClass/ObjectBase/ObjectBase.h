#pragma once

#include <utility>

#include "EntityID.h"

#include "WorldStorage.h"

class ObjectBase
{
public:
	// コンストラクタ
	ObjectBase(WorldStorage* _world, EntityID _entityID) :
		world{ _world },
		id{ _entityID }
	{
	}

	// --- 更新系 ---

	// 更新処理
	virtual void Update() = 0;
	// 物理更新処理
	virtual void FixedUpdate() = 0;

	// --- 衝突系 ---
	
	// 衝突始め
	virtual void OnCollisionEnter() = 0;
	// 衝突中ずっと
	virtual void OnCollision() = 0;
	// 衝突終わり
	virtual void OnCollisionExit() = 0;

	// --- コンポーネント ---

	// 追加
	template<typename T,typename ... Args>
	T* AddComponent(Args&&... args)
	{
		ComponentStorageBase<T>* storage{ world->GetStorage<T>() };

		if(storage != nullptr)
		{
			return storage->Add(id, std::forward<Args>(args)...);
		}

		return nullptr;
	}

	// 取得
	template<typename T>
	T* GetComponent()
	{
		ComponentStorageBase<T>* storage{ world->GetStorage<T>() };

		if (storage != nullptr)
		{
			return storage->TryEdit(id);
		}

		return nullptr;
	}

	// 取得
	template<typename T>
	ComponentView<T> GetComponents()
	{
		ComponentStorageBase<T>* storage{ world->GetStorage<T>() };

		if (storage != nullptr)
		{
			return storage->TryEdits(id);
		}

		return ComponentView<T>{};
	}

	// 除外
	template<typename T>
	void RemoveComponent()
	{
		ComponentStorageBase<T>* storage{ world->GetStorage<T>() };

		if (storage != nullptr)
		{
			storage->Remove(id);
		}
	}

	// 除外
	template<typename T>
	void RemoveComponents()
	{
		ComponentStorageBase<T>* storage{ world->GetStorage<T>() };

		if (storage != nullptr)
		{
			storage->RemoveAll(id);
		}
	}

	// --- ゲッター　---
	EntityID GetID() { return id; }
	// 仮想デストラクタ
	virtual ~ObjectBase() = default;
private:
	EntityID id;
	WorldStorage* world;
};
