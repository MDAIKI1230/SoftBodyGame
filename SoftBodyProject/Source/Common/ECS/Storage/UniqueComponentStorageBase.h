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
	// 取得
	const T& Get(EntityID _entity) const override { return sparseSet.Get(_entity); }
	// 参照取得
	T& Edit(EntityID _entity)override { return sparseSet.Edit(_entity); }
	// 除外
	virtual void Remove(EntityID _entity)override
	{
		const T* component{ TryGet(_entity) };
		if (component != nullptr)
		{
			this->OnRemoving(_entity, *component);
			sparseSet.Remove(_entity);
		}
	}
	// 除外
	virtual void RemoveAll(EntityID _entity)override
	{
		const T* component{ TryGet(_entity) };
		if (component != nullptr)
		{
			this->OnRemoving(_entity, *component);
			sparseSet.Remove(_entity);
		}
	}
	// サイズ生成
	virtual void Reserve(uint32_t _size)override { sparseSet.Reserve(_size); }
	// 全削除
	virtual void Clear()override { sparseSet.Clear(); }
	// 取得できるかトライ
	virtual const T* TryGet(EntityID _entity) const override { return sparseSet.TryGet(_entity); }
	// 取得できるかトライ
	virtual T* TryEdit(EntityID _entity)override { return sparseSet.TryEdit(_entity); }
	// 取得できるかトライ
	virtual ComponentView<T> TryEdits(EntityID _entity)override 
	{
		// 持っていなければ空View
		if (!sparseSet.Has(_entity))
		{
			return {};
		}

		using Self =UniqueComponentStorageBase<T, TABLE>;

		// Viewを作って返すぜぇ
		return ComponentView<T>
		{
			// Uniqueなので0番目の一個だけ
			std::views::iota(
				std::size_t{ 0 },
				std::size_t{ 1 }
			),

			// 条件作成
			ComponentViewGetter<T>
			{
				this,
				_entity,
				[](void* storage, EntityID entity, std::size_t) -> T&
				{
					Self* self{ static_cast<Self*>(storage) };

					return self->Edit(entity);
				}
			}
		};
	}
	// 持っているか
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
	T* AddConstructed(EntityID _entity, T&& _component) override
	{
		return sparseSet.Add(_entity, std::move(_component));
	}
};
