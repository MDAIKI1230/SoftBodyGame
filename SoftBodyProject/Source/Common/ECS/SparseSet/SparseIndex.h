#pragma once

#include <vector>
#include <optional>
#include <span>

template<typename KEY,typename TABLE>
class SparseIndex
{
public:
	using DenseIndex = uint32_t;
public:
	// メモリ確保
	void Reserve(uint32_t _size)
	{
		keys.reserve(_size);
		sparse.Reserve(_size);
	}

	// KEYに対応したやつを探して返してくる
	std::optional<DenseIndex> Find(KEY key) const
	{
		auto denseIndex = sparse.Find(key.GetIndex());

		if (!denseIndex)
		{
			return std::nullopt;
		}

		if (*denseIndex >= keys.size())
		{
			return std::nullopt;
		}

		if (keys[*denseIndex] != key)
		{
			return std::nullopt;
		}

		return denseIndex;
	}

	// 対応する奴がいるかどうか
	bool Contains(KEY _key) const
	{
		return Find(_key).has_value();
	}

	// 追加
	DenseIndex Add(KEY _key)
	{
		DenseIndex index{ static_cast<DenseIndex>(keys.size()) };

		sparse.Set(_key.GetIndex(), index);

		keys.push_back(_key);

		return index;
	}

	// 削除
	std::optional<DenseIndex> Erase(KEY _key)
	{
		const auto removedIndex = Find(_key);

		if (!removedIndex)
		{
			return std::nullopt;
		}

		const DenseIndex index{ *removedIndex };
		const DenseIndex lastIndex{
			static_cast<DenseIndex>(keys.size() - 1)
		};

		if (index != lastIndex)
		{
			const KEY movedKey{ keys.back() };

			keys[index] = movedKey;
			sparse.Set(movedKey.GetIndex(), index);
		}

		keys.pop_back();
		sparse.Erase(_key.GetIndex());

		return index;
	}

	// 全要素削除
	void Clear()
	{
		keys.clear();
		sparse.Clear();
	}

	// サイズ数取得
	uint32_t GetSize() const
	{
		return static_cast<uint32_t>(keys.size());
	}

	// KEY取得
	KEY GetKey(DenseIndex _denseIndex) const
	{
		return keys[_denseIndex];
	}

	// KEY全部返し
	auto GetKeyRange() const
	{
		return std::span{ keys.data(), keys.size()};
	}
private:
	std::vector<KEY> keys;
	TABLE sparse;
};
