#pragma once

#include <span>
#include <ranges>

#include "EntityID.h"

#include "StorageBase.h"

#include "ComponentView.h"

template<class T>
class ComponentStorageBase : public StorageBase
{
public:
	// コンストラクタ
	ComponentStorageBase() = default;
	/// <summary>
	/// 取得
	/// </summary>
	/// <param name="entity">エンティティID</param>
	/// <returns></returns>
	virtual const T& Get(EntityID _entity) const = 0;
	/// <summary>
	/// 参照取得
	/// </summary>
	/// <param name="_entity"></param>
	/// <returns></returns>
	virtual T& Edit(EntityID _entity) = 0;
	/// <summary>
	/// 追加
	/// </summary>
	/// <param name="entity">エンティティID</param>
	template<class... Args>
	T* Add(EntityID _entity, Args&&... _args)
	{
		if (CanAdd(_entity))
		{
			T* result;
			if constexpr (std::is_constructible_v<T, EntityID, Args...>)
			{
				return AddPrepared(_entity, T{ _entity, std::forward<Args>(_args)... });
			}
			else
			{
				return AddPrepared(_entity, T{ std::forward<Args>(_args)... });
			}
			if (!result)
			{
				return nullptr;
			}

			OnAdded();

			return result;
		}

		return nullptr;
	}
	/// <summary>
	/// 除外
	/// </summary>
	/// <param name="entity">エンティティID</param>
	virtual void Remove(EntityID _entity) = 0;
	// サイズ生成
	virtual void Reserve(uint32_t _size) = 0;
	// 全削除
	virtual void Clear() = 0;
	/// <summary>
	/// 取得できるかトライ
	/// </summary>
	/// <returns>無理やったらnullptr</returns>
	virtual const T* TryGet(EntityID _entity) const = 0;
	/// <summary>
	/// 取得できるかトライ
	/// </summary>
	/// <returns>無理やったらnullptr</returns>
	virtual T* TryEdit(EntityID _entity) = 0;
	/// <summary>
	/// 取得できるかトライ
	/// </summary>
	/// <returns>無理やったらnullptr</returns>
	virtual ComponentView<T> TryEdits(EntityID _entity) = 0;
	/// <summary>
	/// 持っているか
	/// </summary>
	/// <param name="target">対象</param>
	/// <returns>持っているか</returns>
	virtual bool Has(EntityID _entity) = 0;
	// サイズ
	virtual size_t GetSize() = 0;
	// 実データコンテナ取得
	virtual std::span<const T> GetDense() const = 0;
	// エンティティコンテナ取得
	virtual std::span<const EntityID> GetEntities() const = 0;
	// 仮想デストラクタ
	virtual ~ComponentStorageBase() = default;
protected:
	// 各種追加処理
	virtual T* AddConstructed(EntityID _entity,T&& _component) = 0;
	// 追加後に即座に呼ばれる関数(追加後必要な処理がるのならここに)
	virtual void OnAdded() {};
	// 追加できるか関数(重複が許されない関数をこれでカットする)
	virtual bool CanAdd(EntityID _entity) { return true; }
private:
	// 追加処理
	T* AddPrepared(EntityID _entity, T&& _component)
	{
		// 各種追加処理を呼ぶ
		T* result{ AddConstructed(_entity, std::move(_component)) };

		if (result)
		{
			OnAdded();
		}

		return result;
	}
};
