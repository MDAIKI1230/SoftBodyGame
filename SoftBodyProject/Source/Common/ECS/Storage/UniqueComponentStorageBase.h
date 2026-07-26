#pragma once

#include "ComponentStorageBase.h"

#include "SparseSet.h"

#include "LinearTable.h"

template<typename T, class TABLE = LinearTable>
class UniqueComponentStorageBase : public ComponentStorageBase<T>
{
public:
	// コンストラクタ
	UniqueComponentStorageBase() = default;
	/// <summary>
	/// 取得
	/// </summary>
	/// <param name="entity">エンティティID</param>
	/// <returns></returns>
	const T& Get(EntityID _entity) const override { return sparseSet.Get(_entity); }
	/// <summary>
	/// 参照取得
	/// </summary>
	/// <param name="_entity"></param>
	/// <returns></returns>
	T& Edit(EntityID _entity)override { return sparseSet.Edit(_entity); }
	/// <summary>
	/// 除外
	/// </summary>
	/// <param name="entity">エンティティID</param>
	virtual void Remove(EntityID _entity)override { sparseSet.Remove(_entity); }
	// サイズ生成
	virtual void Reserve(uint32_t _size)override { sparseSet.Reserve(_size); }
	// 全削除
	virtual void Clear()override { sparseSet.Clear(); }
	/// <summary>
	/// 取得できるかトライ
	/// </summary>
	/// <returns>無理やったらnullptr</returns>
	virtual const T* TryGet(EntityID _entity) const override { return sparseSet.TryGet(_entity); }
	/// <summary>
	/// 取得できるかトライ
	/// </summary>
	/// <returns>無理やったらnullptr</returns>
	virtual T* TryEdit(EntityID _entity)override { return sparseSet.TryEdit(_entity); }
	/// <summary>
	/// 持っているか
	/// </summary>
	/// <param name="target">対象</param>
	/// <returns>持っているか</returns>
	bool Has(EntityID _entity)override { return sparseSet.Has(_entity); }
	// サイズ
	size_t GetSize()override { return sparseSet.GetSize(); }
	// 実データコンテナ取得
	std::span<const T> GetDense() const override { return sparseSet.GetDenseRange(); }
	// エンティティコンテナ取得
	std::span<const EntityID> GetEntities() const override { return sparseSet.GetKeyRange(); }
	// 仮想デストラクタ
	virtual ~UniqueComponentStorageBase() = default;
protected:
	// 追加後に即座に呼ばれる関数(追加後必要な処理がるのならここに)
	virtual void OnAdded()override {};
	// 追加できるか関数(重複が許されない関数をこれでカットする)
	virtual bool CanAdd(EntityID _entity)override { return true; }
protected:
	SparseSet<T, EntityID, TABLE> sparseSet{};
private:
	T* AddConstructed(EntityID entity, T&& component) override
	{
		return sparseSet.Add(entity, std::move(component));
	}
};
