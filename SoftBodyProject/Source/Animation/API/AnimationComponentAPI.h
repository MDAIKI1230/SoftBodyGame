#pragma once

#include "AnimationWorld.h"
#include "RendererComponent.h"

class AnimationComponentAPI
{
public:
	// --- ラグドール ---

	// 作成
	static RagdollID CreateRagdoll(EntityID _entityID, const RendererComponent& _rendererComponent, const std::string& _path);
	// 破棄
	static void DestroyRagdoll(RagdollID _id);

	static void BindWorld(AnimationWorld* _world);
	static void UnbindWorld();
private:
	AnimationComponentAPI() = default;
private:
	static SkeletonInstanceStorage* skeletonStorage;
	static RagdollStorage* ragdollStorage;
};
