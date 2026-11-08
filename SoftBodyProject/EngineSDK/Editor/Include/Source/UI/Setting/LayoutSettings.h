#pragma once

#include <optional>

// 項目の配置と大きさの設定
struct LayoutSettings
{
	// 直前の入力欄と同じセル内に横並びで描画するか
	bool sameLine{ false };
	// 横並びの開始位置。0なら直前の項目の隣に置く
	float offset{ 0.0f };
	// 横並びの間隔。負数なら既定の間隔を使う
	float spacing{ -1.0f };
	// 字下げ幅。未指定なら字下げせず、0なら既定の字下げ幅を使う
	std::optional<float> indent{};
	// 項目の前に追加する縦の空白
	float space{ 0.0f };
	// 項目の前に区切り線を表示するか
	bool separator{ false };
	// 入力欄の幅。未指定なら共通の幅、0なら既定幅、負数なら残り幅から差し引く
	std::optional<float> width{};
	// テーブルの行の最小の高さ
	float rowHeight{ 0.0f };
};