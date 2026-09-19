#pragma once

#include "SkeletonInstanceStorage.h"
#include "AnimationStorage.h"
#include "PoseLayerStorage.h"
#include "RagdollStorage.h"
#include "ActiveRagdollStorage.h"

#include "AnimationSyncSystem.h"
#include "BaseAnimationSystem.h"
#include "TargetPoseSystem.h"
#include "RagdollSystem.h"
#include "ActiveRagdollSystem.h"
#include "AnimationCommitSystem.h"

class AnimationWorld
{
public:
	// 通常アニメーション評価・Pose同期
	void Update(WorldStorage* worldStorage);

	// Physics実行前
	void PrePhysicsFixedUpdate();

	// Physics実行後
	void PostPhysicsFixedUpdate();

	// Rendererが描画する直前に呼ぶ
	void PreRenderUpdate(WorldStorage* worldStorage);

	// --- ゲッター ---

	SkeletonInstanceStorage* GetSkeletonInstanceStorage() { return &skeletonInstanceStorage; }
	AnimationStorage* GetAnimationStorage() { return &animationStorage; }
	PoseLayerStorage* GetPoseLayerStorage() { return &poseLayerStorage; }
	RagdollStorage* GetRagdollStorage() { return &gagdollStorage; }
	ActiveRagdollStorage* GetActiveRagdollStorage() { return &activeRagdollStorage; }
private:
	// --- ストレージ ---

	SkeletonInstanceStorage skeletonInstanceStorage;
	AnimationStorage animationStorage;
	PoseLayerStorage poseLayerStorage;
	RagdollStorage gagdollStorage;
	ActiveRagdollStorage activeRagdollStorage;

	// --- System ---

	AnimationSyncSystem animationSyncSystem;
	TargetPoseSystem targetPoseSystem;
	BaseAnimationSystem baseAnimationSystem;

	RagdollSystem ragdollSystem;
	ActiveRagdollSystem activeRagdollSystem;

	AnimationCommitSystem animationCommitSystem;
};
