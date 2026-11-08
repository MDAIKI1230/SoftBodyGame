#pragma once

#include <cstdint>
#include <optional>

// 配列やリストの入力設定
struct CollectionSettings
{
	// 要素数の最小値。未指定なら下限を設けない
	std::optional<std::size_t> minCount{};
	// 要素数の最大値。未指定なら上限を設けない
	std::optional<std::size_t> maxCount{};
	// 要素を並び替えられるか
	bool reorderable{ false };
	// 現在の要素数を固定し、追加と削除を禁止するか
	bool fixedSize{ false };
};