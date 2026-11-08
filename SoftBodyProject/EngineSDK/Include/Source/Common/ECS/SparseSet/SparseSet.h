#pragma once

#include <vector>
#include <span>
#include <algorithm>

#include "SparseIndex.h"

template<class DENSE, class KEY, class TABLE>
class SparseSet
{
public:
	SparseSet() = default;

	/// <summary>
	/// 取得
	/// </summary>
	/// <param name="_key">ID</param>
	/// <returns></returns>
	const DENSE& Get(KEY _key) const
	{
		return dense[sparse.Find(_key).value()];
	}
	/// <summary>
	/// 取得
	/// </summary>
	/// <param name="_key"></param>
	/// <returns></returns>
	DENSE& Edit(KEY _key)
	{
		return dense[sparse.Find(_key).value()];
	}
	/// <summary>
	/// 追加
	/// </summary>
	/// <param name="_key">ID</param>
	template<class... Args>
	DENSE* Add(KEY _key, Args&&... _args)
	{
		// KEYがないかチェック
		if (sparse.Contains(_key))
		{
			return nullptr;
		}

		// ねぇなら追加
		dense.emplace_back(std::forward<Args>(_args)...);
		sparse.Add(_key);

		return &dense.back();
	}
	/// <summary>
	/// 除外
	/// </summary>
	/// <param name="_key">ID</param>
	void Remove(KEY _key)
	{
		// 除外インデックス(対応もなくす)
		auto denseIndex{ sparse.Erase(_key) };
		// ないなら何もしない
		if (denseIndex == std::nullopt)
		{
			return;
		}

		const size_t index{ *denseIndex };
		const size_t lastIndex{ dense.size() - 1 };

		if (index != lastIndex)
		{
			dense[index] = std::move(dense.back());
		}

		// 実データを除外
		dense.pop_back();
	}
	// メモリ確保
	void Reserve(uint32_t _size)
	{
		// コンテナのreserve関数を呼ぶ
		dense.reserve(_size);
		sparse.Reserve(_size);
	}
	// 全削除
	void Clear()
	{
		// 全部クリア
		dense.clear();
		sparse.Clear();
	}
	// 取得できるかわからんが取得
	const DENSE* TryGet(KEY _key) const
	{
		// KEYに対応するIndexを取得
		const auto denseIndex{ sparse.Find(_key) };

		// 存在チェック
		if (!denseIndex)
		{
			return nullptr;
		}

		return &dense[*denseIndex];
	}
	// 取得できるかわからんが取得
	DENSE* TryEdit(KEY _key)
	{
		// KEYに対応するIndexを取得
		const auto denseIndex{ sparse.Find(_key) };

		// 存在チェック
		if (!denseIndex)
		{
			return nullptr;
		}

		return &dense[*denseIndex];
	}
	/// <summary>
	/// 持っているか
	/// </summary>
	/// <param name="_key">対象ID</param>
	/// <returns>持っているか</returns>
	bool Has(KEY _key) const
	{
		return sparse.Contains(_key);
	}
	// サイズ
	size_t GetSize() const
	{
		// denseのサイズを返す
		return dense.size();
	}
	// キャパ
	size_t GetCapacity() const
	{
		// denseのキャパを返す
		return dense.capacity();
	}
	// 実データコンテナ取得
	auto GetDenseRange() const
	{
		return std::span{ dense.data(), dense.size() };
	}
	// エンティティコンテナ取得
	auto GetKeyRange() const
	{
		return sparse.GetKeyRange();
	}

	~SparseSet() = default;
private:
	// 実データ
	std::vector<DENSE> dense;
	// 対応表
	SparseIndex<KEY, TABLE> sparse;
};
