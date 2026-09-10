#pragma once

#include "SkeletonInstanceStorage.h"
#include "RagdollStorage.h"

#include "AnimationCommitSystem.h"
#include "RagdollSystem.h"
#include "AnimationSyncSystem.h"

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
	RagdollStorage* GetRagdollStorage() { return &gagdollStorage; }
private:
	// --- ストレージ ---

	SkeletonInstanceStorage skeletonInstanceStorage;
	RagdollStorage gagdollStorage;

	// --- System ---

	AnimationCommitSystem animationCommitSystem;

	RagdollSystem ragdollSystem;

	AnimationSyncSystem animationSyncSystem;
};
