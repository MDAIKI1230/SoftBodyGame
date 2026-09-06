#pragma once

#include "EntityID.h"
#include "SkeletonID.h"

#include "Buffer/PoseBuffer.h"

struct SkeletonInstanceData
{
    EntityID owner;

    // 共有される骨格定義
    SkeletonID skeleton;

    // このキャラクター専用
    PoseBuffer targetPose;
    PoseBuffer outputPose;
};
