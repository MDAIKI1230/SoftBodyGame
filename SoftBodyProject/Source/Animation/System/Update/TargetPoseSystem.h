#pragma once

#include "SkeletonInstanceStorage.h"

/*
	スケルトンインスタンスデータのターゲットポーズを作るところ
	後からIKなどが追加されてもそのまま使えるようにする。
	今日はターゲットポーズにTposeを入れて終わり。
*/

class TargetPoseSystem
{
public:
	void Update(SkeletonInstanceStorage* _skeletonStorage);
};