#pragma once

// 入力する値と同じ型のリセット設定
template<class Value>
struct ResetSettings
{
	// リセット時に書き戻す値を返す関数。未指定ならリセット操作を表示しない
	Value (*getValue)(){ nullptr };
};