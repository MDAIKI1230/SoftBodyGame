#pragma once

// コンポーネントや連続する項目のまとまりの設定
struct GroupSettings
{
	// グループ名。未指定なら新しいグループを作らない
	const char* name{ nullptr };
	// 項目の前に表示する、折り畳まない見出し
	const char* header{ nullptr };
	// コンポーネントやグループを折り畳めるか
	bool foldout{ true };
	// 初めて表示したときに開いておくか
	bool defaultOpen{ true };
	// 見出しを選択可能にするか
	bool selectable{ false };
	// 現在の選択状態。呼び出し側が描画ごとに設定する
	bool selected{ false };
	// 見出しの右上に閉じるボタンを表示するか
	bool closable{ false };
};