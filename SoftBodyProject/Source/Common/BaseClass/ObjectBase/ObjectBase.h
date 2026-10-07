#pragma once

#include <utility>

#include "EntityID.h"

#include "WorldStorage.h"

class ObjectBase
{
public:
	// コンストラクタ
	ObjectBase(WorldStorage* _world, EntityID _entityID);
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
	EntityID GetID() const { return id; }
	// 仮想デストラクタ
	virtual ~ObjectBase() = default;
private:
	EntityID id;
	WorldStorage* world;
};
