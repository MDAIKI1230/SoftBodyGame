#pragma once

#include <optional>

// 数値入力の設定。入力する値と同じ型で範囲や刻み幅を保持する
template<class Value>
struct NumberSettings
{
	// 数値の入力方法
	enum class Mode
	{
		Drag,
		Slider,
		Input
	};

	// Inputは文字入力。Drag/Sliderではmin/maxに応じてSliderかDragを自動選択する
	Mode mode{ Mode::Drag };
	// 最小値。未指定なら下限を設けない
	std::optional<Value> min{};
	// 最大値。未指定なら上限を設けない
	std::optional<Value> max{};
	// ドラッグで値を変更するときの速度
	float speed{ 0.1f };
	// 増減操作の刻み幅。未指定なら入力部品の既定値を使う
	std::optional<Value> step{};
	// 高速な増減操作の刻み幅。未指定なら入力部品の既定値を使う
	std::optional<Value> fastStep{};
	// 数値の表示書式。未指定なら値の型に合う書式を使う
	const char* format{ nullptr };
	// 値の変化を対数スケールにするか
	bool logarithmic{ false };
	// 直接入力した値も指定範囲に収めるか
	bool clamp{ true };
	// ドラッグやスライダーから文字入力に切り替えられるか
	bool directInput{ true };
};

// 小数の入力設定
using FloatSettings = NumberSettings<float>;
// 整数の入力設定
using IntSettings = NumberSettings<int>;