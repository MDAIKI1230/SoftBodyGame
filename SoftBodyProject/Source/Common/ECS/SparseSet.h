#pragma once

#include <vector>
#include <unordered_map>
#include <algorithm>

template<typename T>
class SparseSet
{
public:
	SparseSet() = default;

	/// <summary>
	/// 取得
	/// </summary>
	/// <param name="entity">エンティティID</param>
	/// <returns></returns>
	T* Get(EntityID _entity)
	{
		size_t id{ sparse[_entity] };
		return &dense[id];
	}
	/// <summary>
	/// 追加
	/// </summary>
	/// <param name="entity">エンティティID</param>
	template<class... Args>
	T* Add(EntityID _entity, Args&&... _args)
	{
		// 追加インデックスを作成
		size_t index{ dense.size() };
		// コンポーネント追加
		dense.emplace_back(std::forward<Args>(_args)...);
		// エンティティ追加
		entities.push_back(_entity);
		// 対応付け
		sparse[_entity] = index;

		return &dense.back();
	}
	/// <summary>
	/// 除外
	/// </summary>
	/// <param name="entity">エンティティID</param>
	void Remove(EntityID _entity)
	{
		// 除外コンポーネントインデックス
		size_t denseIndex{ sparse[_entity] };
		// コンポーネントを除外
		dense[denseIndex] = std::move(dense.back());
		dense.pop_back();
		// ID削除
		EntityID movedEntity{ entities.back() };
		entities[denseIndex] = std::move(entities.back());
		entities.pop_back();
		// MAP対応更新
		sparse[movedEntity] = denseIndex;
		// MAPから除外
		sparse.erase(_entity);
	}
	// メモリ確保
	void Reserve(size_t _size)
	{
		// コンテナのreserve関数を呼ぶ
		dense.reserve(_size);
		entities.reserve(_size);
	}
	// サイズ分生成
	void Resize(size_t _size)
	{
		// コンテナのreserve関数を呼ぶ
		dense.resize(_size);
		entities.resize(_size);
	}

	// 全削除
	void Clear()
	{
		// 全部クリア
		dense.clear();
		entities.clear();
		sparse.clear();
	}
	/// <summary>
	/// 取得できるか
	/// </summary>
	/// <param name="output">取得したコンポーネント</param>
	/// <returns>取得できたか</returns>
	bool TryGet(EntityID _entity, T& _output)
	{
		// 空チェック
		if (sparse.empty())
		{
			return false;
		}

		// エンティティがあるかチェック
		if (sparse.contains(_entity))
		{
			// ある場合はアウトプットに入れてtrueを返す
			_output = dense[sparse[_entity]];
			return true;
		}

		// ここまで来たらfalseを返す
		return false;
	}
	/// <summary>
	/// IDがあるかどうか
	/// </summary>
	/// <param name="_entity">ID</param>
	/// <returns></returns>
	bool TryGet(EntityID _entity)
	{
		return sparse.contains(_entity);
	}
	/// <summary>
	/// 持っているか
	/// </summary>
	/// <param name="target">対象</param>
	/// <returns>持っているか</returns>
	bool Has(EntityID _entity)
	{
		// 空チェック
		if (sparse.empty())
		{
			return false;
		}

		// エンティティがあるかチェック
		if (sparse.contains(_entity))
		{
			// ある場合はtrueを返す
			return true;
		}

		// ここまで来たらfalseを返す
		return false;
	}
	// サイズ
	size_t GetSize()
	{
		// 代表してdenseのサイズを返す(すべて同じ値になっている)
		return dense.size();
	}
	// キャパ
	size_t GetCapacity()
	{
		// 代表してdenseのキャパを返す(すべて同じ値になっている)
		return dense.capacity();
	}
	// 実データコンテナ取得
	std::vector<T>* GetDense()
	{
		return &dense;
	}
	// エンティティコンテナ取得
	std::vector<EntityID>* GetEntities()
	{
		return &entities;
	}

	~SparseSet() = default;
private:
	// 実データ
	std::vector<T> dense{};
	// エンティティ
	std::vector<EntityID> entities{};
	// 対応マップ
	std::unordered_map<EntityID, size_t> sparse{};
};
