#include "AnimationWorld.h"

// 通常アニメーション評価・Pose同期
void AnimationWorld::Update(WorldStorage* worldStorage)
{
	animationSyncSystem.Sync(&skeletonInstanceStorage, worldStorage);
	baseAnimationSystem.Update(&animationStorage, &poseLayerStorage);
	targetPoseSystem.Update(&skeletonInstanceStorage, &poseLayerStorage);
}

// Physics実行前
void AnimationWorld::PrePhysicsFixedUpdate()
{
	ragdollSystem.PrePhysicsFixedUpdate(&skeletonInstanceStorage, &gagdollStorage);
	activeRagdollSystem.PrePhysicsFixedUpdate(&skeletonInstanceStorage, &gagdollStorage, &activeRagdollStorage);
}

// Physics実行後
void AnimationWorld::PostPhysicsFixedUpdate()
{
	ragdollSystem.PostPhysicsFixedUpdate(&skeletonInstanceStorage, &gagdollStorage);
}

// Rendererが描画する直前に呼ぶ
void AnimationWorld::PreRenderUpdate(WorldStorage* worldStorage)
{
	animationCommitSystem.Commit(&skeletonInstanceStorage, worldStorage);
}
