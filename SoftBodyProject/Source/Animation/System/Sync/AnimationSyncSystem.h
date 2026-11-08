#pragma once

#include "SkeletonInstanceStorage.h"
#include "WorldStorage.h"

/*
	他で起こった変更をAnimation内のデータにも反映するためのシステム
*/
class AnimationSyncSystem
{
public:
	void Sync(SkeletonInstanceStorage* _skeletonStorage, WorldStorage* _worldStorage);
};
