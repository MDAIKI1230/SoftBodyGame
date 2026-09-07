#pragma once

#include "EntityID.h"

#include "Buffer/PoseBuffer.h"

struct SkeletonInstanceData
{
    EntityID owner;

    // このキャラクター専用
    PoseBuffer targetPose;
    PoseBuffer outputPose;
};
