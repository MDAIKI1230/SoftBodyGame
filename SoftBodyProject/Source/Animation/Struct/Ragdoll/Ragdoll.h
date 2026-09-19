#pragma once

#include <vector>

#include "SkeletonID.h"
#include "ConstraintID.h"
#include "ColliderID.h"
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
	// コライダー
	std::vector<ColliderID> colliders;
	// 物理トランスフォーム
	std::vector<PhysicsTransformID> transforms;
	// Ragdoll上の親ボーンインデックス
	std::vector<uint32_t> parentIndices;
};
