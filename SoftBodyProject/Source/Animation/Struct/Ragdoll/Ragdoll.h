#pragma once

#include <vector>

#include "SkeletonID.h"
#include "ConstraintID.h"
#include "PhysicsTransformID.h"

#include "RagdollBodyLink.h"

/*
    Ragdollの実データ
	リンクをすべてのボーンに対して用意してRagdollSystemの分岐処理を楽にする予定
*/
struct Ragdoll
{
    SkeletonID skeleton;

    // RagdollとPhysicsのデータの共有データ
    std::vector<RagdollBodyLink> bodyLinks;
    // 拘束
    std::vector<ConstraintID> constraints;
    // 物理トランスフォーム
    std::vector<PhysicsTransformID> transforms;
};
