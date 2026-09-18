#pragma once

#include "SkeletonInstanceStorage.h"
#include "RagdollStorage.h"
#include "ActiveRagdollStorage.h"

/*
	今日は一旦スケルトンインスタンスデータのターゲットポーズを取るように動く拘束を発生させる感じ
	各関節駆動拘束に対して正しい角度を入れてあげるのが今日までの仕事
*/
class ActiveRagdollSystem
{
public:
	void PrePhysicsFixedUpdate(SkeletonInstanceStorage* _skeletonStorage, RagdollStorage* _ragdollStorage, ActiveRagdollStorage* _activeRagdollStorage);
};