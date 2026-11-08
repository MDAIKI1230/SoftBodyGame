#pragma once

// 色入力の設定
struct ColorSettings
{
	// 自動描画側が、ベクトル入力ではなくRGB/RGBAの色入力を選ぶか
	bool enabled{ false };
	// 明るさが1を超えるHDRの値を扱うか
	bool hdr{ false };
};