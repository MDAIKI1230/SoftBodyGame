#include "ResourceManager.h"

#include "AnimationComponentAPI.h"

// --- 通常アニメーション ---

// 生成
AnimationID AnimationComponentAPI::CreateAnimation(EntityID _entity, const RendererComponent* _rendererComponent, const char* _path)
{
	ModelHandle model{ _rendererComponent->GetHandle() };
	SkeletonID skeletonID{ skeletonStorage->CreateOrGetID(_entity,model) };
	PoseLayerID poseLayerID{ poseLayerStorage->Create(
		skeletonID,
		skeletonStorage->GetSkeletonInstanceData(skeletonID),
		_path) };

	return animationStorage->Create(model, poseLayerID);
}
// 破棄
void AnimationComponentAPI::DestroyAnimation(AnimationID _id)
{
	animationStorage->Destroy(_id);
}

// アニメーションの名前の取得
std::string_view AnimationComponentAPI::GetAnimationName(AnimationID _id)
{
	return animationStorage->GetAnimationInstanceData(_id).animationName;
}
// アニメーションの名前の変更
void AnimationComponentAPI::SetAnimationName(AnimationID _id, const char* _path)
{
	AnimationInstanceData& animation{ animationStorage->EditAnimationInstanceData(_id) };
	animation.animationName = _path;

	animation.animationHandle =
		ResourceManager::AttachAnimation(
			animation.modelHandle,
			animation.animationName);


	animation.total = ResourceManager::GetAnimTotalTime(
		animation.modelHandle,
		animation.animationHandle);

}

// アニメーション再生時間取得
float AnimationComponentAPI::GetTime(AnimationID _id)
{
	return animationStorage->GetAnimationInstanceData(_id).time;
}
// アニメーション再生時間変更
void AnimationComponentAPI::SetTime(AnimationID _id, float _time)
{
	animationStorage->EditAnimationInstanceData(_id).time = _time;
}
// アニメーションスピード取得
float AnimationComponentAPI::GetSpeed(AnimationID _id)
{
	return animationStorage->GetAnimationInstanceData(_id).speed;
}
// アニメーションスピード変更
void AnimationComponentAPI::SetSpeed(AnimationID _id, float _speed)
{
	animationStorage->EditAnimationInstanceData(_id).speed = _speed;
}

// ループ再生フラグ取得
bool AnimationComponentAPI::GetLoop(AnimationID _id)
{
	return animationStorage->GetAnimationInstanceData(_id).loop;
}
// ループ再生フラグ変更
void AnimationComponentAPI::SetLoop(AnimationID _id, bool _isLoop)
{
	animationStorage->EditAnimationInstanceData(_id).loop = _isLoop;
}
// 再生フラグ取得
bool AnimationComponentAPI::GetPlaying(AnimationID _id)
{
	return animationStorage->GetAnimationInstanceData(_id).playing;
}
// 再生フラグ変更
void AnimationComponentAPI::SetPlaying(AnimationID _id, bool _isPlay)
{
	animationStorage->EditAnimationInstanceData(_id).playing = _isPlay;
}

// --- ラグドール ---

// ラグドール作成
RagdollID AnimationComponentAPI::CreateRagdoll(EntityID _entityID, const RendererComponent& _rendererComponent, const std::string& _path)
{
	SkeletonID skeletonID{ skeletonStorage->CreateOrGetID(_entityID,_rendererComponent.GetHandle()) };
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
	SkeletonID skeletonID{ skeletonStorage->CreateOrGetID(_entityID,_rendererComponent.GetHandle()) };
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
