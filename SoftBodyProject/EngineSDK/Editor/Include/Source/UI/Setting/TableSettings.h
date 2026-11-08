#pragma once

#include <optional>

// テーブルのセル内の余白
struct CellPaddingSettings
{
	// 横の余白
	float x{ 4.0f };
	// 縦の余白
	float y{ 2.0f };
};

// 項目名と入力欄を並べるテーブルの設定
struct TableSettings
{
	// 左の項目名の列幅の比率
	float labelWeight{ 0.4f };
	// 右の入力欄の列幅の比率
	float valueWeight{ 0.6f };
	// 左の列の固定幅。未指定なら比率で幅を決める
	std::optional<float> labelWidth{};
	// セル内の余白。未指定なら現在のスタイルを使う
	std::optional<CellPaddingSettings> cellPadding{};
	// 列幅をマウスで変更できるか
	bool resizableColumns{ false };
	// 外枠と列や行の境界線を表示するか
	bool borders{ false };
	// 行の背景色を交互に変えるか
	bool rowBackground{ false };
};