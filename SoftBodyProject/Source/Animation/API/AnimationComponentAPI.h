#pragma once

#include "AnimationWorld.h"
#include "RendererComponent.h"

class AnimationComponentAPI
{
public:
	// --- 通常アニメーション ---

	// 生成
	static AnimationID CreateAnimation(EntityID _entity, const RendererComponent* _rendererComponent);
	// 破棄
	static void DestroyAnimation(AnimationID _id);

	// --- ラグドール ---

	// 作成
	static RagdollID CreateRagdoll(EntityID _entityID, const RendererComponent& _rendererComponent, const std::string& _path);
	// 破棄
	static void DestroyRagdoll(RagdollID _id);

	// --- アクティブラグドール ---

	// 作成
	static ActiveRagdollID CreateActiveRagdoll(EntityID _entityID, const RendererComponent& _rendererComponent, const std::string& _path);
	// 破棄
	static void DestroyActiveRagdoll(ActiveRagdollID _id);

	static void BindWorld(AnimationWorld* _world);
	static void UnbindWorld();
private:
	AnimationComponentAPI() = default;
private:
	static SkeletonInstanceStorage* skeletonStorage;
	static AnimationStorage* animationStorage;
	static PoseLayerStorage* poseLayerStorage;
	static RagdollStorage* ragdollStorage;
	static ActiveRagdollStorage* activeRagdollStorage;
};
