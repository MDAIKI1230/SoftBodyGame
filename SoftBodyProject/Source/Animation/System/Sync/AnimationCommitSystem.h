#pragma once

#include "SkeletonInstanceStorage.h"
#include "WorldStorage.h"

/*
	Animationによって起きた変更をワールドストレージに反映する
*/
class AnimationCommitSystem
{
public:
	void Commit(SkeletonInstanceStorage* _skeletonStorage, WorldStorage* _worldStorage);
private:
	// ポーズの行列を位置/回転/スケールから作り直す
	void ReBuildMatrices(PoseBuffer& _poseBuffer);
};
