#pragma once

#include "RagdollStorage.h"
#include "SkeletonInstanceStorage.h"

/*
	計算開始時前にRigidBodyの情報をRagdoll側に写したり
	RigidBodyの結果をPoseにしたりする。
*/
class RagdollSystem
{
public:
	// Physics実行前
	void PrePhysicsFixedUpdate(SkeletonInstanceStorage* _skeletonStorage, RagdollStorage* _ragdollStorage);
	// Physics実行後
	void PostPhysicsFixedUpdate(SkeletonInstanceStorage* _skeletonStorage, RagdollStorage* _ragdollStorage);
};
