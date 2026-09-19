#include "AnimationComponentAPI.h"

// --- 通常アニメーション ---

// 生成
AnimationID AnimationComponentAPI::CreateAnimation(EntityID _entity, const RendererComponent* _rendererComponent)
{
	ModelHandle model{ _rendererComponent->GetHandle() };
	SkeletonID skeletonID{ skeletonStorage->CreateOrGet(_entity,model) };
	PoseLayerID poseLayerID{ poseLayerStorage->Create(skeletonID, skeletonStorage->GetSkeletonInstanceData(skeletonID).targetPose) };

	return animationStorage->Create(model, poseLayerID);
}
// 破棄
void AnimationComponentAPI::DestroyAnimation(AnimationID _id)
{
	animationStorage->Destroy(_id);
}

// --- ラグドール ---

// ラグドール作成
RagdollID AnimationComponentAPI::CreateRagdoll(EntityID _entityID, const RendererComponent& _rendererComponent, const std::string& _path)
{
	SkeletonID skeletonID{ skeletonStorage->CreateOrGet(_entityID,_rendererComponent.GetHandle()) };
	return ragdollStorage->Create(
		_entityID, skeletonID,
		_rendererComponent.GetHandle(),
		skeletonStorage->GetSkeletonInstanceData(skeletonID),
		_path);
}

// 破棄
void AnimationComponentAPI::DestroyRagdoll(RagdollID _id)
{
	skeletonStorage->Destroy(ragdollStorage->GetRagdoll(_id).skeleton);
	ragdollStorage->Destroy(_id);
}

// --- アクティブラグドール ---

	// 作成
ActiveRagdollID AnimationComponentAPI::CreateActiveRagdoll(EntityID _entityID, const RendererComponent& _rendererComponent, const std::string& _path)
{
	SkeletonID skeletonID{ skeletonStorage->CreateOrGet(_entityID,_rendererComponent.GetHandle()) };
	const SkeletonInstanceData& skeleton{ skeletonStorage->GetSkeletonInstanceData(skeletonID) };

	RagdollID ragdollID{ ragdollStorage->Create(
		_entityID,skeletonID,
		_rendererComponent.GetHandle(),
		skeleton,
		_path) };

	return activeRagdollStorage->Create(
		_entityID,
		ragdollID,
		ragdollStorage->GetRagdoll(ragdollID),
		skeleton,
		_path);
}
// 破棄
void AnimationComponentAPI::DestroyActiveRagdoll(ActiveRagdollID _id)
{
	activeRagdollStorage->Destroy(_id);
}

void AnimationComponentAPI::BindWorld(AnimationWorld* _world)
{
	skeletonStorage = _world->GetSkeletonInstanceStorage();
	animationStorage = _world->GetAnimationStorage();
	poseLayerStorage = _world->GetPoseLayerStorage();
	ragdollStorage = _world->GetRagdollStorage();
	activeRagdollStorage = _world->GetActiveRagdollStorage();
}

void AnimationComponentAPI::UnbindWorld()
{
	skeletonStorage = nullptr;
	animationStorage = nullptr;
	poseLayerStorage = nullptr;
	ragdollStorage = nullptr;
	activeRagdollStorage = nullptr;
}

SkeletonInstanceStorage* AnimationComponentAPI::skeletonStorage{ nullptr };
AnimationStorage* AnimationComponentAPI::animationStorage{ nullptr };
PoseLayerStorage* AnimationComponentAPI::poseLayerStorage{ nullptr };
RagdollStorage* AnimationComponentAPI::ragdollStorage{ nullptr };
ActiveRagdollStorage* AnimationComponentAPI::activeRagdollStorage{ nullptr };
