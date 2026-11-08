#pragma once

#include <cstdint>
#include <optional>

// 文字列入力の設定
struct TextSettings
{
	// 入力が空のときに表示する入力例
	const char* hint{ nullptr };
	// 複数行の入力欄を使うか
	bool multiline{ false };
	// 複数行入力で確保する行数
	int lines{ 4 };
	// 最大バイト数。終端文字を含まず、未指定なら制限しない
	std::optional<std::size_t> maxLength{};
};