#pragma once

#include <type_traits>

#include "EntityID.h"

#include "StorageBase.h"
#include "SparseSet.h"

template<typename T>
class SparseSetStorageBase :public StorageBase
{
public:
	// コンストラクタ
	SparseSetStorageBase() = default;
	/// <summary>
	/// 取得
	/// </summary>
	/// <param name="entity">エンティティID</param>
	/// <returns></returns>
	virtual T* Get(EntityID _entity) { return sparseSet.Get(_entity); }
	/// <summary>
	/// 追加
	/// </summary>
	/// <param name="entity">エンティティID</param>
	template<class... Args>
		T* Add(EntityID _entity, Args&&... _args)
		{
			T* result;
			if constexpr (std::is_constructible_v<T, EntityID, Args...>)
			{
				result = sparseSet.Add(_entity, _entity, std::forward<Args>(_args)...);
			}
			else
			{
				result = sparseSet.Add(_entity, std::forward<Args>(_args)...);
			}
			OnAdded();
			return result;
		}
	/// <summary>
	/// 除外
	/// </summary>
	/// <param name="entity">エンティティID</param>
	virtual void Remove(EntityID _entity) { sparseSet.Remove(_entity); }
	// サイズ生成
	virtual void Reserve(size_t _size) { sparseSet.Reserve(_size); }
	// 全削除
	virtual void Clear() { sparseSet.Clear(); }
	/// <summary>
	/// 取得できるか
	/// </summary>
	/// <param name="output">取得したコンポーネント</param>
	/// <returns>取得できたか</returns>
	virtual bool TryGet(EntityID _entity, T& _output) { return sparseSet.TryGet(_entity, _output); }
	/// <summary>
	/// IDがあるかどうか
	/// </summary>
	virtual bool TryGet(EntityID _entity) { return sparseSet.TryGet(_entity); }
	/// <summary>
	/// 持っているか
	/// </summary>
	/// <param name="target">対象</param>
	/// <returns>持っているか</returns>
	virtual bool Has(EntityID _entity) { return sparseSet.Has(_entity); }
	// サイズ
	virtual size_t GetSize() { return sparseSet.GetSize(); }
	// 実データコンテナ取得
	virtual std::vector<T>* GetDense() { return sparseSet.GetDense(); }
	// エンティティコンテナ取得
	virtual std::vector<EntityID>* GetEntities() { return sparseSet.GetEntities(); }
	// ハンドル取得
	virtual size_t GetHandle() { return sparseSet.GetSize(); }

	// 仮想デストラクタ
	virtual ~SparseSetStorageBase() = default;
protected:
	virtual void OnAdded() {};
protected:
	SparseSet<T> sparseSet{};
};
