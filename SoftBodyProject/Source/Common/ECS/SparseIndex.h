#pragma once

#include <vector>
#include <optional>

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
	std::optional<DenseIndex> Find(Key key) const
	{
		auto denseIndex = table.Find(key.GetIndex());

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
	bool Contains(Key key) const
	{
		return Find(key).has_value();
	}
	// 追加
	DenseIndex Add(KEY _key)
	{
		DenseIndex index{ static_cast<DenseIndex>(keys.size()) };

		sparse.Set(index.GetIndex(), _key);

		keys.push_back(_key);

		return index;
	}
	// 削除
	void Erase(KEY _key)
	{
		for (int i{ 0 }; i < keys.size(); i++)
		{
			if (keys[i] == _key)
			{
				keys.erase(i);
			}
		}

		sparse.Erase(_key);
	}
private:
	std::vector<KEY> keys;
	TABLE sparse;
};
