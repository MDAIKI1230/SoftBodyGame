#pragma once

#include <array>

// ベクトル入力の設定。数値の範囲や速度はNumberSettingsで扱う
struct VectorSettings
{
	// 各成分に表示する名前
	std::array<const char*, 4> axisLabels{ "X", "Y", "Z", "W" };
};