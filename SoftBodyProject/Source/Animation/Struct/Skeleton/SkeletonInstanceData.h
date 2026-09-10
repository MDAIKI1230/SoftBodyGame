#pragma once

#include "SkeletonData.h"

#include "Buffer/PoseBuffer.h"

struct SkeletonInstanceData
{
    // おおもとのスケルトンデータ
    const SkeletonData* skeletonData;

    // このキャラクター専用
    PoseBuffer targetPose;
    PoseBuffer outputPose;
};
