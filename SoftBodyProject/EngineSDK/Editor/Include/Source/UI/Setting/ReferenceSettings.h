#pragma once

// オブジェクトやアセットへの参照入力の設定
struct ReferenceSettings
{
	// 選択する参照先の種類
	enum class Kind
	{
		None,
		Asset,
		Object,
		Component
	};

	// 未指定なら参照選択の入力欄を使わない
	Kind kind{ Kind::None };
	// アセットやコンポーネントの選択対象の種類名
	const char* typeName{ nullptr };
	// 参照先を未設定にできるか
	bool allowNull{ true };
};