#pragma once

// パス入力と選択ダイアログの設定
struct PathSettings
{
	// 選択するパスの種類
	enum class Kind
	{
		None,
		File,
		Directory
	};

	// 未指定なら通常の文字列として扱う
	Kind kind{ Kind::None };
	// ファイル選択で表示する拡張子の条件
	const char* filter{ nullptr };
	// 現在のパスが空、またはファイル名だけを保持する場合に開く初期フォルダ
	const char* initialDirectory{ nullptr };
	// ファイルパスの代わりに、拡張子付きファイル名だけを保持する
	bool fileNameOnly{ false };
};