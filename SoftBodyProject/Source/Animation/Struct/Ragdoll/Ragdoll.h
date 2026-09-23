#pragma once

#include <array>
#include <vector>

#include "RagdollConstants.h"

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
	Ragdoll()
	{
		roles.fill(UINT32_MAX);
	}

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
	// 役割ボーン対応表(RagdollBoneRoleの順番で入れていく)
	std::array<uint32_t, static_cast<size_t>(RagdollBoneRole::COUNT)> roles;
};
