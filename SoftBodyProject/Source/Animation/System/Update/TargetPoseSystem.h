#pragma once

#include "SkeletonInstanceStorage.h"
#include "PoseLayerStorage.h"

/*
	スケルトンインスタンスデータのターゲットポーズを作るところ
	後からIKなどが追加されてもそのまま使えるようにする。
	今日はターゲットポーズにTposeを入れて終わり。
*/

class TargetPoseSystem
{
public:
	void Update(SkeletonInstanceStorage* _skeletonStorage);

private:
	void InitialePose(SkeletonInstanceStorage* _skeletonStorage);

	void MixPose(SkeletonInstanceStorage* _skeletonStorage, PoseLayerStorage* _poseLayerStorage);
};