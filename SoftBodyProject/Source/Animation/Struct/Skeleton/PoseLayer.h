#pragma once

#include "Buffer/PoseBuffer.h"
#include "BoneMask.h"
#include "SkeletonID.h"

/*
	各システム等で作成する、ボーンをどう動かしたいかを個別で設定するための構造体
	ここに格納したポーズとマスク情報を使って各ボーンのターゲットポーズを作成する
	アニメーション優先度も入れる
*/
struct PoseLayer
{
	// 対象スケルトン
	SkeletonID skeletonID;

	// ターゲットポーズに入れるポーズ
	PoseBuffer pose;

	// ボーンのマスク(重み)
	BoneMask mask;

	// 全体の重み
	float weight{ 1.0f };

	// 優先度
	int priority{ 0 };
};