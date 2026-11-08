#pragma once

#include <array>
#include <cstdint>

#include "MDGuiOption.h"

// Enum入力の設定。選択肢の型と数を保ち、選択肢の配列も保持する
template<class Value, std::size_t Count = 0>
struct EnumSettings
{
	// 選択肢に表示する名前と、対応する値
	std::array<MDGuiOption::EnumItem<Value>, Count> items{};
	// ビットフラグとして複数の項目を選択できるか
	bool flags{ false };
};