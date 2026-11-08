#pragma once

// Inspectorの各項目に共通する表示と操作の設定
struct RowSettings
{
	// 左側の項目名。未指定ならフィールド名を使う
	const char* label{ nullptr };
	// 入力欄側に表示する名前
	const char* inputLabel{ "##Value" };
	// 項目にマウスを重ねたときの説明文
	const char* tooltip{ nullptr };
	// 項目に表示する単位名
	const char* unit{ nullptr };
	// 項目を表示するか。条件付き表示も判定後の結果を入れる
	bool visible{ true };
	// 入力操作を有効にするか。条件付き操作も判定後の結果を入れる
	bool enabled{ true };
	// 値の表示だけを行い、編集を禁止するか
	bool readOnly{ false };
	// 左側の項目名を隠すか
	bool hideLabel{ false };
};