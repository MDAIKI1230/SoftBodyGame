#pragma once

#include <ranges>

#include "ComponentStorageBase.h"

#include "SparseSet.h"
#include "ComponentIDPool.h"

#include "LinearTable.h"

template<typename T, class TABLE = LinearTable>
class MultiComponentStorageBase : public ComponentStorageBase<T>
{
public:
	// コンストラクタ
	MultiComponentStorageBase() = default;
	// 取得
	const T& Get(EntityID _entity) const override
	{ 
		uint32_t index{ idPool.GetDenseIndex(_entity) };
		return dense[index];
	}
	// 取得
	auto GetAll(EntityID _entity) const
	{ 
		return idPool.GetComponentIDs(_entity)
			| std::views::transform(
				[this](ComponentID<T> id) -> const T&
				{
					return dense[idPool.GetDenseIndex(id)];
				});
	}
	///参照取得
	T& Edit(EntityID _entity)override
	{
		uint32_t index{ idPool.GetDenseIndex(_entity) };
		return dense[index];
	}
	// 除外
	virtual void Remove(EntityID _entity)override
	{ 
		const auto id{ idPool.GetComponentIDs(_entity).back() };

		Remove(id);
	}
	// 除外
	void Remove(ComponentID<T> _id)
	{
		const uint32_t denseIndex{ idPool.Remove(_id) };
		const uint32_t lastIndex{ static_cast<uint32_t>(dense.size() - 1) };

		if (denseIndex == ComponentID<T>::INVALID_INDEX)
		{
			return;
		}

		if (denseIndex != lastIndex)
		{
			dense[denseIndex] = std::move(dense.back());
		}

		dense.pop_back();
	}
	// サイズ生成
	virtual void Reserve(uint32_t _size)override
	{ 
		idPool.Reserve(_size);
		dense.reserve(_size);
	}
	// 全削除
	virtual void Clear()override
	{
		idPool.Clear();
		dense.clear();
	}
	// 取得できるかトライ
	virtual const T* TryGet(EntityID _entity) const override
	{ 
		if (!idPool.Has(_entity))
		{
			return nullptr;
		}
		
		uint32_t index{ idPool.GetDenseIndex(_entity) };

		return &dense[index];
	}
	// 取得できるかトライ
	virtual T* TryEdit(EntityID _entity) override
	{
		if (!idPool.Has(_entity))
		{
			return nullptr;
		}

		uint32_t index{ idPool.GetDenseIndex(_entity) };

		return &dense[index];
	}
	// 持っているか
	bool Has(EntityID _entity) override
	{ 
		return idPool.Has(_entity);
	}
	// サイズ
	size_t GetSize() override
	{
		return dense.size();
	}
	// 実データコンテナ取得
	std::span<const T> GetDense() const override { return { dense.data(),dense.size() }; }
	// エンティティコンテナ取得
	std::span<const EntityID> GetEntities() const override { return idPool.GetOwnerRange(); }
	// 仮想デストラクタ
	virtual ~MultiComponentStorageBase() = default;
protected:
	// 追加後に即座に呼ばれる関数(追加後必要な処理がるのならここに)
	virtual void OnAdded()override {};
	// 追加できるか関数(重複が許されない関数をこれでカットする)
	virtual bool CanAdd(EntityID _entity)override { return true; }
protected:
	ComponentIDPool<T, TABLE> idPool;
	std::vector<T> dense;
private:
	T* AddConstructed(EntityID _entity, T&& _component) override
	{
		uint32_t index{ static_cast<uint32_t>(dense.size()) };

		dense.push_back(std::move(_component));
		idPool.Add(index, _entity);

		return &dense.back();
	}
};
