#pragma once

#include <vector>
#include <unordered_map>

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
	T* Get(int _entity)
	{
		int id{ sparse[_entity] };
		return &dense[id];
	}
	/// <summary>
	/// 追加
	/// </summary>
	/// <param name="entity">エンティティID</param>
	/// <param name="component">追加オブジェクト</param>
	void Add(int _entity, const T& _obj)
	{
		// コンポーネント追加
		dense.push_back(_obj);
		// エンティティ追加
		entities.push_back(_entity);
		// 対応付け
		sparse[_entity] = dense.size() - 1;
	}
	/// <summary>
	/// 除外
	/// </summary>
	/// <param name="entity">エンティティID</param>
	void Remove(int _entity)
	{
		// 除外コンポーネントインデックス
		int denseIndex{ sparse[_entity] };
		// コンポーネントを除外
		dense[denseIndex] = std::move(dense.back());
		dense.pop_back();
		// ID削除
		entities[denseIndex] = std::move(entities.back());
		entities.pop_back();
		// MAPから除外
		sparse.erase(_entity);
	}
	// サイズ生成
	void Reserve(size_t _size)
	{
		// コンテナのreserve関数を呼ぶ
		dense.reserve(_size);
		entities.reserve(_size);
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
	bool TryGet(int _entity, T& _output)
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
	/// 持っているか
	/// </summary>
	/// <param name="target">対象</param>
	/// <returns>持っているか</returns>
	bool Has(int _entity)
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
	// 実データコンテナ取得
	std::vector<T>* GetDense()
	{
		return &dense;
	}
	// エンティティコンテナ取得
	std::vector<int>* GetEntities()
	{
		return &entities;
	}

	~SparseSet() = default;
private:
	// 実データ
	std::vector<T> dense{};
	// エンティティ
	std::vector<int> entities{};
	// 対応マップ
	std::unordered_map<int, int> sparse{};
};