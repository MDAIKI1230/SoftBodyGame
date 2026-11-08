#pragma once

#include <vector>
#include <span>
#include <algorithm>

#include "ComponentID.h"
#include "EntityID.h"

#include "SparseSet.h"

template<class T, class TABLE>
class ComponentIDPool
{
public:
	// メモリ確保
	void Reserve(uint32_t _size)
	{
		pool.reserve(_size);
		ownerIDs.reserve(_size);
		freeSlots.reserve(_size);
		denseIndices.reserve(_size);
		generations.reserve(_size);
		entityComponentIDs.Reserve(_size);
	}
	// 対応DenseIndexの取得
	uint32_t GetDenseIndex(ComponentID<T> _id) const
	{
		return denseIndices[_id.GetIndex()];
	}
	// 対応DenseIndexの取得
	uint32_t GetDenseIndex(EntityID _id) const
	{
		return denseIndices[entityComponentIDs.Get(_id)[0].GetIndex()];
	}
	// 対応EntityID取得
	EntityID GetOwner(ComponentID<T> _id) const
	{
		return ownerIDs[_id.GetIndex()];
	}
	// EntityIDがコンポーネントを持ってるかチェック
	bool Has(EntityID entity) const
	{
		return entityComponentIDs.Has(entity);
	}
	// 対応コンポーネントIDすべて取得
	std::span<const ComponentID<T>>GetComponentIDs(EntityID entity) const
	{
		const auto& ids = entityComponentIDs.Get(entity);
		return { ids.data(), ids.size() };
	}
	// 追加処理
	ComponentID<T> Add(uint32_t _denseIndex, EntityID _entityID)
	{
		ComponentID<T> result;

		// 空きあるなら空きから作る
		if (freeSlots.empty())
		{
			// インデックス
			uint32_t index{ static_cast<uint32_t>(denseIndices.size()) };
			// 世代
			uint32_t generation{ 1 };

			// 新たな情報作成
			denseIndices.push_back(_denseIndex);
			ownerIDs.push_back(_entityID);
			generations.emplace_back(1);

			// 結果を保持
			result = ComponentID<T>{ index, generation };

			pool.push_back(result);
		}
		else
		{
			// インデックス
			uint32_t index{ freeSlots.back() };
			// 世代
			uint32_t generation{ generations[index] };

			// フリースロットなくす
			freeSlots.pop_back();

			// 新たな情報修正
			denseIndices[index] = _denseIndex;
			ownerIDs[index] = _entityID;

			// 結果を保持
			result = ComponentID<T>{ index, generation };

			pool.push_back(result);
		}

		// 対応付け
		auto idPool{ entityComponentIDs.TryEdit(_entityID) };
		if (idPool == nullptr)
		{
			idPool = entityComponentIDs.Add(_entityID);
		}

		idPool->push_back(result);

		return result;
	}
	// 対応IDの削除
	uint32_t Remove(ComponentID<T> _id)
	{
		// 生きてるIDなのかチェック
		if (!_id.IsValid())
		{
			return ComponentID<T>::INVALID_INDEX;
		}
		// 世代チェックして正しい値か調べる
		if (generations[_id.GetIndex()] != _id.GetGeneration())
		{
			return ComponentID<T>::INVALID_INDEX;
		}

		EntityID ownerID{ GetOwner(_id) };
		// EntityIDに対応するComponentIDが無くなってたら削除処理じゃ。
		auto* idPools{ entityComponentIDs.TryEdit(ownerID) };

		if (idPools)
		{
			auto it = std::find(idPools->begin(), idPools->end(), _id);

			if (it != idPools->end())
			{
				*it = idPools->back();
				idPools->pop_back();
			}

			if (idPools->empty())
			{
				entityComponentIDs.Remove(ownerID);
			}
		}

		// スワップ＆ポップ
		const uint32_t denseIndex{ denseIndices[_id.GetIndex()] };
		const uint32_t lastIndex{ static_cast<uint32_t>(pool.size() - 1) };

		if (denseIndex != lastIndex)
		{
			const auto movedID = pool.back();

			pool[denseIndex] = movedID;
			denseIndices[movedID.GetIndex()] = denseIndex;
		}

		pool.pop_back();

		// 世代アップ
		generations[_id.GetIndex()]++;
		// フリーに追加
		freeSlots.push_back(_id.GetIndex());

		return denseIndex;
	}
	// 要素全削除
	void Clear()
	{
		pool.clear();
		ownerIDs.clear();
		freeSlots.clear();
		denseIndices.clear();
		generations.clear();
		entityComponentIDs.Clear();
	}
	// EntityID全取得
	std::span<const EntityID> GetOwnerRange() const
	{
		return entityComponentIDs.GetKeyRange();
	}
private:
	std::vector<ComponentID<T>> pool;
	std::vector<EntityID> ownerIDs;
	std::vector<uint32_t> freeSlots;
	std::vector<uint32_t> denseIndices;
	std::vector<uint32_t> generations;
	// EntityIDから、ComponentIDへの対応
	SparseSet<std::vector<ComponentID<T>>, EntityID, TABLE> entityComponentIDs;
};
