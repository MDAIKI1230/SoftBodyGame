#pragma once

#include <typeindex>
#include <memory>

#include "SparseSetStorageBase.h"

class WorldStorage
{
public:
	/// <summary>
	/// ストレージ取得
	/// </summary>
	/// <typeparam name="T">取得したいストレージ</typeparam>
	/// <returns></returns>
	template<typename T>
	SparseSetStorageBase<T>* GetStorage()
	{
		auto it{ storageMap.find(std::type_index(typeid(T))) };

		if (it == storageMap.end())
		{
			return nullptr;
		}

		return static_cast<SparseSetStorageBase<T>*>(storages[it->second].get());
	}

	// デストラクタ
	~WorldStorage() = default;
protected:
	// ストレージ
	std::vector<std::unique_ptr<StorageBase>> storages;
	// ストレージと型の対応マップ
	std::unordered_map<std::type_index, size_t> storageMap;
};
