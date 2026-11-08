#pragma once

// モデルのメッシュに含まれる変形の分類
enum class ModelGeometryType
{
	// 情報取得失敗や未対応形式などで判定できない
	UNKNOWN,
	// スキニングとモーフを含まない剛体メッシュのみ
	RIGID,
	// スキニングまたはモーフを含む
	DEFORMABLE
};