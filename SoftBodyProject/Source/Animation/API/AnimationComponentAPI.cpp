#include "ResourceManager.h"

#include "ActiveRagdollLoader.h"

#include "ActiveRagdollDefinition.h"

#include "AnimationComponentAPI.h"
#include "SolverIKStorage.h"

// --- 通常アニメーション ---

// 生成
AnimationID AnimationComponentAPI::CreateAnimation(EntityID _entity, const RendererComponent* _rendererComponent, const char* _path)
{
	ModelHandle model{ _rendererComponent->GetHandle() };
	SkeletonID skeletonID{ skeletonStorage->CreateOrGetID(_entity,model) };
	PoseLayerID poseLayerID{ poseLayerStorage->Create(
		skeletonID,
		skeletonStorage->GetSkeletonDataPtr(skeletonID),
		skeletonStorage->GetTargetPose(skeletonID),
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
		skeletonStorage->GetSkeletonDataPtr(skeletonID),
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
	const SkeletonData* skeleton{ skeletonStorage->GetSkeletonDataPtr(skeletonID) };

	ActiveRagdollDefinition definition;
	if (!ActiveRagdollLoader::Load(_path, definition))
	{
		return {};
	}

	RagdollID ragdollID{ ragdollStorage->Create(
		_entityID,skeletonID,
		_rendererComponent.GetHandle(),
		skeleton,
		definition.ragdollDefinition) };

	return activeRagdollStorage->Create(
		_entityID,
		ragdollID,
		ragdollStorage->GetRagdoll(ragdollID),
		skeleton,
		definition.settings);
}
// 破棄
void AnimationComponentAPI::DestroyActiveRagdoll(ActiveRagdollID _id)
{
	RagdollID ragdollID{ activeRagdollStorage->GetRagdollID(_id) };
	DestroyRagdoll(ragdollID);
	activeRagdollStorage->Destroy(_id);
}

// --- IK ---

// ハンドIK作成
FeatureIKID AnimationComponentAPI::CreateHandIK(EntityID _entity, const RendererComponent* _rendererComponent)
{
	ModelHandle model{ _rendererComponent->GetHandle() };
	SkeletonID skeletonID{ skeletonStorage->CreateOrGetID(_entity, model) };

	PoseLayerID poseLayerID{ poseLayerStorage->Create(
		skeletonID,
		skeletonStorage->GetSkeletonDataPtr(skeletonID),
		skeletonStorage->GetTargetPose(skeletonID),
		nullptr) };

	SolverIKID solverIKID{ solverIKStorage->CreateTwoBoneIK(poseLayerID, skeletonID) };

	return featureIKStorage->CreateHandIK(solverIKID, skeletonID);
}

// IK破棄
void AnimationComponentAPI::DestroyIK(FeatureIKID _id)
{
	SolverIKID solverIKID{ featureIKStorage->GetHandSolverIKID(_id) };
	PoseLayerID poseLayerID{ solverIKStorage->GetTwoBoneIKPoseLayerID(solverIKID) };

	featureIKStorage->Destroy(_id);
	solverIKStorage->Destroy(solverIKID);
	poseLayerStorage->Destroy(poseLayerID);
}

// 上腕ボーン取得
std::string_view AnimationComponentAPI::GetUpperArm(FeatureIKID _id)
{
	SolverIKID solverIKID{ featureIKStorage->GetHandSolverIKID(_id) };
	uint32_t boneIndex{ solverIKStorage->GetTwoBoneIK(solverIKID).rootBoneIndex };

	return GetHandBoneName(_id, boneIndex);
}

// 上腕ボーン変更
void AnimationComponentAPI::SetUpperArm(FeatureIKID _id, const char* _boneName)
{
	SolverIKID solverIKID{ featureIKStorage->GetHandSolverIKID(_id) };
	solverIKStorage->EditTwoBoneIK(solverIKID).rootBoneIndex = FindHandBoneIndex(_id, _boneName);
}

// 前腕ボーン取得
std::string_view AnimationComponentAPI::GetLowerArm(FeatureIKID _id)
{
	SolverIKID solverIKID{ featureIKStorage->GetHandSolverIKID(_id) };
	uint32_t boneIndex{ solverIKStorage->GetTwoBoneIK(solverIKID).jointBoneIndex };

	return GetHandBoneName(_id, boneIndex);
}

// 前腕ボーン変更
void AnimationComponentAPI::SetLowerArm(FeatureIKID _id, const char* _boneName)
{
	SolverIKID solverIKID{ featureIKStorage->GetHandSolverIKID(_id) };
	solverIKStorage->EditTwoBoneIK(solverIKID).jointBoneIndex = FindHandBoneIndex(_id, _boneName);
}

// 手ボーン取得
std::string_view AnimationComponentAPI::GetHand(FeatureIKID _id)
{
	SolverIKID solverIKID{ featureIKStorage->GetHandSolverIKID(_id) };
	uint32_t boneIndex{ solverIKStorage->GetTwoBoneIK(solverIKID).endBoneIndex };

	return GetHandBoneName(_id, boneIndex);
}

// 手ボーン変更
void AnimationComponentAPI::SetHand(FeatureIKID _id, const char* _boneName)
{
	SolverIKID solverIKID{ featureIKStorage->GetHandSolverIKID(_id) };
	solverIKStorage->EditTwoBoneIK(solverIKID).endBoneIndex = FindHandBoneIndex(_id, _boneName);
}

// ターゲット位置取得
Vector3 AnimationComponentAPI::GetTargetPosition(FeatureIKID _id)
{
	SolverIKID solverIKID{ featureIKStorage->GetHandSolverIKID(_id) };
	return solverIKStorage->GetTwoBoneIK(solverIKID).targetPosition;
}

// ターゲット位置変更
void AnimationComponentAPI::SetTargetPosition(FeatureIKID _id, const Vector3& _targetPosition)
{
	SolverIKID solverIKID{ featureIKStorage->GetHandSolverIKID(_id) };
	solverIKStorage->EditTwoBoneIK(solverIKID).targetPosition = _targetPosition;
}

// ターゲット回転取得
Quaternion AnimationComponentAPI::GetTargetRotation(FeatureIKID _id)
{
	return featureIKStorage->GetHandIK(_id).targetRotation;
}

// ターゲット回転変更
void AnimationComponentAPI::SetTargetRotation(FeatureIKID _id, const Quaternion& _targetRotation)
{
	featureIKStorage->EditHandIK(_id).targetRotation = _targetRotation;
}

// ポール位置取得
Vector3 AnimationComponentAPI::GetPolePosition(FeatureIKID _id)
{
	SolverIKID solverIKID{ featureIKStorage->GetHandSolverIKID(_id) };
	return solverIKStorage->GetTwoBoneIK(solverIKID).polePosition;
}

// ポール位置変更
void AnimationComponentAPI::SetPolePosition(FeatureIKID _id, const Vector3& _polePosition)
{
	SolverIKID solverIKID{ featureIKStorage->GetHandSolverIKID(_id) };
	solverIKStorage->EditTwoBoneIK(solverIKID).polePosition = _polePosition;
}

// 位置ウェイト取得
float AnimationComponentAPI::GetPositionWeight(FeatureIKID _id)
{
	SolverIKID solverIKID{ featureIKStorage->GetHandSolverIKID(_id) };
	return solverIKStorage->GetTwoBoneIK(solverIKID).positionWeight;
}

// 位置ウェイト変更
void AnimationComponentAPI::SetPositionWeight(FeatureIKID _id, float _positionWeight)
{
	SolverIKID solverIKID{ featureIKStorage->GetHandSolverIKID(_id) };
	solverIKStorage->EditTwoBoneIK(solverIKID).positionWeight = _positionWeight;
}

// 回転ウェイト取得
float AnimationComponentAPI::GetRotationWeight(FeatureIKID _id)
{
	return featureIKStorage->GetHandIK(_id).rotationWeight;
}

// 回転ウェイト変更
void AnimationComponentAPI::SetRotationWeight(FeatureIKID _id, float _rotationWeight)
{
	featureIKStorage->EditHandIK(_id).rotationWeight = _rotationWeight;
}

// --- private ---

// 名前からボーンインデックス取得
uint32_t AnimationComponentAPI::FindHandBoneIndex(FeatureIKID _id, const char* _boneName)
{
	SolverIKID solverIKID{ featureIKStorage->GetHandSolverIKID(_id) };
	SkeletonID skeletonID{ solverIKStorage->GetTwoBoneIKSkeletonID(solverIKID) };
	const SkeletonData* skeleton{ skeletonStorage->GetSkeletonDataPtr(skeletonID) };

	return skeleton->boneLookup.at(_boneName);
}

// ボーンインデックスから名前取得
std::string_view AnimationComponentAPI::GetHandBoneName(FeatureIKID _id, uint32_t _boneIndex)
{
	SolverIKID solverIKID{ featureIKStorage->GetHandSolverIKID(_id) };
	SkeletonID skeletonID{ solverIKStorage->GetTwoBoneIKSkeletonID(solverIKID) };
	const SkeletonData* skeleton{ skeletonStorage->GetSkeletonDataPtr(skeletonID) };

	return skeleton->boneNames.at(_boneIndex);
}

void AnimationComponentAPI::BindWorld(AnimationWorld* _world)
{
	skeletonStorage = _world->GetSkeletonInstanceStorage();
	animationStorage = _world->GetAnimationStorage();
	poseLayerStorage = _world->GetPoseLayerStorage();
	ragdollStorage = _world->GetRagdollStorage();
	activeRagdollStorage = _world->GetActiveRagdollStorage();
	solverIKStorage = _world->GetSolverIKStorage();
	featureIKStorage = _world->GetFeatureIKStorage();
}

void AnimationComponentAPI::UnbindWorld()
{
	skeletonStorage = nullptr;
	animationStorage = nullptr;
	poseLayerStorage = nullptr;
	ragdollStorage = nullptr;
	activeRagdollStorage = nullptr;
	solverIKStorage = nullptr;
	featureIKStorage = nullptr;
}

SkeletonInstanceStorage* AnimationComponentAPI::skeletonStorage{ nullptr };
AnimationStorage* AnimationComponentAPI::animationStorage{ nullptr };
PoseLayerStorage* AnimationComponentAPI::poseLayerStorage{ nullptr };
RagdollStorage* AnimationComponentAPI::ragdollStorage{ nullptr };
ActiveRagdollStorage* AnimationComponentAPI::activeRagdollStorage{ nullptr };
SolverIKStorage* AnimationComponentAPI::solverIKStorage{ nullptr };
FeatureIKStorage* AnimationComponentAPI::featureIKStorage{ nullptr };