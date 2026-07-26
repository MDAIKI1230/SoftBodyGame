#pragma once

#include <typeindex>
#include <memory>
#include <unordered_map>

#include "ComponentStorageBase.h"
#include "UniqueComponentStorageBase.h"

class WorldStorage
{
public:
	/// <summary>
	/// ストレージ取得
	/// </summary>
	/// <typeparam name="T">取得したいストレージ</typeparam>
	/// <returns></returns>
	template<typename T>
	typename ComponentStorageBase<T>* GetStorage()
	{
		auto it{ storageMap.find(std::type_index(typeid(T))) };

		if (it == storageMap.end())
		{
			return nullptr;
		}

		return static_cast<ComponentStorageBase<T>*>(storages[it->second].get());
	}

	/// <summary>
	/// ストレージの追加(moveされる)
	/// </summary>
	/// <param name="storage">入れたいストレージ</param>
	template<typename T>
	void AddStorage(std::unique_ptr<StorageBase>&& _storage)
	{
		// コンテナに追加
		storages.push_back(std::move(_storage));
		// 対応付け
		storageMap[typeid(T)] = storages.size() - 1;
	}

	// デストラクタ
	~WorldStorage() = default;
protected:
	// ストレージ
	std::vector<std::unique_ptr<StorageBase>> storages;
	// ストレージと型の対応マップ
	std::unordered_map<std::type_index, size_t> storageMap;
};
