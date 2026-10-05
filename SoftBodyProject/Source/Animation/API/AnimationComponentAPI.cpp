#include <algorithm>
#include <cmath>

#include "ResourceManager.h"

#include "ActiveRagdollLoader.h"
#include "BoneMaskLoader.h"

#include "ActiveRagdollDefinition.h"

#include "AnimationComponentAPI.h"
#include "PhysicsComponentAPI.h"
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

// アクティブフラグ取得
bool AnimationComponentAPI::GetActive(AnimationID _id)
{
	PoseLayerID layerID{ animationStorage->GetAnimationInstanceData(_id).layerID };

	return poseLayerStorage->GetPoseLayer(layerID).isActive;
}

// アクティブフラグ変更
void AnimationComponentAPI::SetActive(AnimationID _id, bool _isActive)
{
	PoseLayerID layerID{ animationStorage->GetAnimationInstanceData(_id).layerID };

	poseLayerStorage->EditPoseLayer(layerID).isActive = _isActive;
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

	if (definition.boneMaskPath.empty())
	{
		return activeRagdollStorage->Create(
			_entityID,
			ragdollID,
			ragdollStorage->GetRagdoll(ragdollID),
			skeleton,
			definition.settings);
	}

	BoneMask boneMask;
	if (BoneMaskLoader::Load(definition.boneMaskPath.c_str(), skeleton, boneMask))
	{
		return activeRagdollStorage->Create(
			_entityID,
			ragdollID,
			ragdollStorage->GetRagdoll(ragdollID),
			skeleton,
			definition.settings,
			&boneMask);
	}
	else
	{
		return activeRagdollStorage->Create(
			_entityID,
			ragdollID,
			ragdollStorage->GetRagdoll(ragdollID),
			skeleton,
			definition.settings);
	}
}
// 破棄
void AnimationComponentAPI::DestroyActiveRagdoll(ActiveRagdollID _id)
{
	RagdollID ragdollID{ activeRagdollStorage->GetRagdollID(_id) };
	DestroyRagdoll(ragdollID);
	activeRagdollStorage->Destroy(_id);
}

// 名前からボーンインデックス取得
uint32_t AnimationComponentAPI::GetActiveRagdollBoneIndex(ActiveRagdollID _id, const char* _boneName)
{
	if (!activeRagdollStorage->IsAlive(_id) || _boneName == nullptr)
	{
		return UINT32_MAX;
	}

	RagdollID ragdollID{ activeRagdollStorage->GetRagdollID(_id) };
	const Ragdoll& ragdoll{ ragdollStorage->GetRagdoll(ragdollID) };
	const SkeletonData* skeleton{ skeletonStorage->GetSkeletonDataPtr(ragdoll.skeleton) };

	if (skeleton == nullptr)
	{
		return UINT32_MAX;
	}

	const auto it{ skeleton->boneLookup.find(_boneName) };
	return it != skeleton->boneLookup.end() ? it->second : UINT32_MAX;
}

// 役割からボーンインデックス取得
uint32_t AnimationComponentAPI::GetActiveRagdollBoneIndex(ActiveRagdollID _id, RagdollBoneRole _role)
{
	if (!activeRagdollStorage->IsAlive(_id))
	{
		return UINT32_MAX;
	}

	RagdollID ragdollID{ activeRagdollStorage->GetRagdollID(_id) };
	const Ragdoll& ragdoll{ ragdollStorage->GetRagdoll(ragdollID) };

	size_t roleIndex{ static_cast<size_t>(_role) };
	return roleIndex < ragdoll.roles.size() ? ragdoll.roles[roleIndex] : UINT32_MAX;
}

// インデックス指定で個別変更
bool AnimationComponentAPI::SetActiveRagdollWeight(ActiveRagdollID _id, uint32_t _boneIndex, float _weight)
{
	return SetActiveRagdollWeightInternal(_id, _boneIndex, _weight, false);
}

// 役割指定で個別変更
bool AnimationComponentAPI::SetActiveRagdollWeight(ActiveRagdollID _id, RagdollBoneRole _role, float _weight)
{
	return SetActiveRagdollWeight(_id, GetActiveRagdollBoneIndex(_id, _role), _weight);
}

// インデックス指定で自身と子孫を変更
bool AnimationComponentAPI::SetActiveRagdollBranchWeight(ActiveRagdollID _id, uint32_t _rootBoneIndex, float _weight)
{
	return SetActiveRagdollWeightInternal(_id, _rootBoneIndex, _weight, true);
}

// 役割指定で自身と子孫を変更
bool AnimationComponentAPI::SetActiveRagdollBranchWeight(ActiveRagdollID _id, RagdollBoneRole _rootRole, float _weight)
{
	return SetActiveRagdollBranchWeight(_id, GetActiveRagdollBoneIndex(_id, _rootRole), _weight);
}

bool AnimationComponentAPI::SetActiveRagdollPositionWeight(ActiveRagdollID _id, uint32_t _boneIndex, float _weight)
{
	return SetActiveRagdollWeightInternal(_id, _boneIndex, _weight, false, true, false);
}

bool AnimationComponentAPI::SetActiveRagdollPositionWeight(ActiveRagdollID _id, RagdollBoneRole _role, float _weight)
{
	return SetActiveRagdollPositionWeight(_id, GetActiveRagdollBoneIndex(_id, _role), _weight);
}

bool AnimationComponentAPI::SetActiveRagdollRotationWeight(ActiveRagdollID _id, uint32_t _boneIndex, float _weight)
{
	return SetActiveRagdollWeightInternal(_id, _boneIndex, _weight, false, false, true);
}

bool AnimationComponentAPI::SetActiveRagdollRotationWeight(ActiveRagdollID _id, RagdollBoneRole _role, float _weight)
{
	return SetActiveRagdollRotationWeight(_id, GetActiveRagdollBoneIndex(_id, _role), _weight);
}

bool AnimationComponentAPI::SetActiveRagdollBranchPositionWeight(ActiveRagdollID _id, uint32_t _rootBoneIndex, float _weight)
{
	return SetActiveRagdollWeightInternal(_id, _rootBoneIndex, _weight, true, true, false);
}

bool AnimationComponentAPI::SetActiveRagdollBranchPositionWeight(ActiveRagdollID _id, RagdollBoneRole _rootRole, float _weight)
{
	return SetActiveRagdollBranchPositionWeight(_id, GetActiveRagdollBoneIndex(_id, _rootRole), _weight);
}

bool AnimationComponentAPI::SetActiveRagdollBranchRotationWeight(ActiveRagdollID _id, uint32_t _rootBoneIndex, float _weight)
{
	return SetActiveRagdollWeightInternal(_id, _rootBoneIndex, _weight, true, false, true);
}

bool AnimationComponentAPI::SetActiveRagdollBranchRotationWeight(ActiveRagdollID _id, RagdollBoneRole _rootRole, float _weight)
{
	return SetActiveRagdollBranchRotationWeight(_id, GetActiveRagdollBoneIndex(_id, _rootRole), _weight);
}

// インデックス指定で目標位置を設定
bool AnimationComponentAPI::SetActiveRagdollTargetPosition(ActiveRagdollID _id, uint32_t _boneIndex, const Vector3& _worldPosition)
{
	if (!activeRagdollStorage->IsAlive(_id) || !std::isfinite(_worldPosition.x) || !std::isfinite(_worldPosition.y) || !std::isfinite(_worldPosition.z))
	{
		return false;
	}

	RagdollID ragdollID{ activeRagdollStorage->GetRagdollID(_id) };
	if (!ragdollStorage->IsAlive(ragdollID))
	{
		return false;
	}

	SkeletonID skeletonID{ ragdollStorage->GetRagdoll(ragdollID).skeleton };
	if (!skeletonStorage->IsAlive(skeletonID))
	{
		return false;
	}

	const SkeletonData* skeleton{ skeletonStorage->GetSkeletonDataPtr(skeletonID) };
	if (skeleton == nullptr || _boneIndex >= skeleton->Size())
	{
		return false;
	}

	auto& positionTargets{ skeletonStorage->EditPositionTargets(skeletonID) };
	for (PositionTarget& target : positionTargets)
	{
		if (target.boneIndex == _boneIndex)
		{
			target.worldPosition = _worldPosition;
			return true;
		}
	}

	positionTargets.push_back(PositionTarget{ _boneIndex, _worldPosition });
	return true;
}

// 役割指定で目標位置を設定
bool AnimationComponentAPI::SetActiveRagdollTargetPosition(ActiveRagdollID _id, RagdollBoneRole _role, const Vector3& _worldPosition)
{
	return SetActiveRagdollTargetPosition(_id, GetActiveRagdollBoneIndex(_id, _role), _worldPosition);
}

// インデックス指定で位置指定を解除
bool AnimationComponentAPI::ClearActiveRagdollTargetPosition(ActiveRagdollID _id, uint32_t _boneIndex)
{
	if (!activeRagdollStorage->IsAlive(_id))
	{
		return false;
	}

	RagdollID ragdollID{ activeRagdollStorage->GetRagdollID(_id) };
	if (!ragdollStorage->IsAlive(ragdollID))
	{
		return false;
	}

	SkeletonID skeletonID{ ragdollStorage->GetRagdoll(ragdollID).skeleton };
	if (!skeletonStorage->IsAlive(skeletonID))
	{
		return false;
	}

	const SkeletonData* skeleton{ skeletonStorage->GetSkeletonDataPtr(skeletonID) };
	if (skeleton == nullptr || _boneIndex >= skeleton->Size())
	{
		return false;
	}

	auto& positionTargets{ skeletonStorage->EditPositionTargets(skeletonID) };
	for (auto it{ positionTargets.begin() }; it != positionTargets.end(); ++it)
	{
		if (it->boneIndex == _boneIndex)
		{
			positionTargets.erase(it);
			return true;
		}
	}

	return false;
}

// 役割指定で位置指定を解除
bool AnimationComponentAPI::ClearActiveRagdollTargetPosition(ActiveRagdollID _id, RagdollBoneRole _role)
{
	return ClearActiveRagdollTargetPosition(_id, GetActiveRagdollBoneIndex(_id, _role));
}

bool AnimationComponentAPI::GetActiveRagdollBoneWorldPosition(ActiveRagdollID _id, uint32_t _boneIndex, Vector3& _worldPosition)
{
	if (!activeRagdollStorage->IsAlive(_id))
	{
		return false;
	}

	RagdollID ragdollID{ activeRagdollStorage->GetRagdollID(_id) };
	if (!ragdollStorage->IsAlive(ragdollID))
	{
		return false;
	}

	// Bodyの初期姿勢が設定されるまでは取得しない
	if (ragdollStorage->GetNeedInitialize(ragdollID))
	{
		return false;
	}

	const Ragdoll& ragdoll{ ragdollStorage->GetRagdoll(ragdollID) };
	if (_boneIndex >= ragdoll.bodyLinks.size())
	{
		return false;
	}

	const RagdollBodyLink& link{ ragdoll.bodyLinks[_boneIndex] };
	Matrix4x4 worldFromBone;

	if (link.bodyID.IsValid())
	{
		// RagdollSystemと同じ変換でBodyのオフセットを戻す
		Matrix4x4 worldFromBody{ MatGenerateFunc::TRS(
			PhysicsComponentAPI::GetRigidBodyPosition(link.bodyID),
			PhysicsComponentAPI::GetRigidBodyRoatation(link.bodyID),
			Vector3::ONE) };

		worldFromBone = worldFromBody * link.bodyFromBone;
	}
	else
	{
		// Bodyを持たないボーンは最終ポーズから取得する
		if (!skeletonStorage->IsAlive(ragdoll.skeleton))
		{
			return false;
		}

		const PoseBuffer& outputPose{
			skeletonStorage->GetOutputPose(ragdoll.skeleton)
		};

		if (_boneIndex >= outputPose.modelFromBoneMatrices.size())
		{
			return false;
		}

		worldFromBone =
			skeletonStorage->GetWorldFromModel(ragdoll.skeleton) *
			outputPose.modelFromBoneMatrices[_boneIndex];
	}

	_worldPosition = Vector3{
		worldFromBone.m[0][3],
		worldFromBone.m[1][3],
		worldFromBone.m[2][3]
	};

	return true;
}

bool AnimationComponentAPI::GetActiveRagdollBoneWorldPosition(ActiveRagdollID _id, RagdollBoneRole _role, Vector3& _worldPosition)
{
	return GetActiveRagdollBoneWorldPosition(_id, GetActiveRagdollBoneIndex(_id, _role), _worldPosition);
}

// --- Body取得 ---

// 指定した役割のRigidBody取得
RigidBodyComponent AnimationComponentAPI::GetActiveRagdollBody(
	ActiveRagdollID _id,
	RagdollBoneRole _role)
{
	RagdollID ragdollID{ activeRagdollStorage->GetRagdollID(_id) };
	const Ragdoll& ragdoll{ ragdollStorage->GetRagdoll(ragdollID) };
	uint32_t boneIndex{ ragdoll.roles[static_cast<size_t>(_role)] };

	return RigidBodyComponent{ ragdoll.bodyLinks[boneIndex].bodyID };
}

// --- 操作要求 ---

// ワールド空間の移動入力取得
const Vector3& AnimationComponentAPI::GetActiveRagdollMoveInput(ActiveRagdollID _id)
{
	return activeRagdollStorage->GetActiveRagdoll(_id).moveInput;
}

// ワールド空間の移動入力設定
void AnimationComponentAPI::SetActiveRagdollMoveInput(
	ActiveRagdollID _id,
	const Vector3& _moveInput)
{
	activeRagdollStorage->EditActiveRagdoll(_id).moveInput = _moveInput;
}

// ジャンプ要求
void AnimationComponentAPI::RequestActiveRagdollJump(ActiveRagdollID _id)
{
	activeRagdollStorage->EditActiveRagdoll(_id).jumpRequested = true;
}

// --- 状態取得 ---

// 制御状態取得
ActiveRagdollControlState AnimationComponentAPI::GetActiveRagdollControlState(ActiveRagdollID _id)
{
	return activeRagdollStorage->GetActiveRagdoll(_id).controlState;
}

// 接地状態取得
bool AnimationComponentAPI::GetActiveRagdollGrounded(ActiveRagdollID _id)
{
	return activeRagdollStorage->GetActiveRagdoll(_id).isGrounded;
}

// 地面法線取得
const Vector3& AnimationComponentAPI::GetActiveRagdollGroundNormal(ActiveRagdollID _id)
{
	return activeRagdollStorage->GetActiveRagdoll(_id).groundNormal;
}

// 水平速度取得
const Vector3& AnimationComponentAPI::GetActiveRagdollPlanarVelocity(ActiveRagdollID _id)
{
	return activeRagdollStorage->GetActiveRagdoll(_id).planarVelocity;
}

// 重心位置取得
const Vector3& AnimationComponentAPI::GetActiveRagdollCenterOfMass(ActiveRagdollID _id)
{
	return activeRagdollStorage->GetActiveRagdoll(_id).centerOfMass;
}

// 直立度取得
float AnimationComponentAPI::GetActiveRagdollUprightDot(ActiveRagdollID _id)
{
	return activeRagdollStorage->GetActiveRagdoll(_id).uprightDot;
}

// Ragdoll全体を無視する衝突フィルター取得
CollisionFilter AnimationComponentAPI::GetActiveRagdollIgnoreFilter(ActiveRagdollID _id)
{
	RagdollID ragdollID{ activeRagdollStorage->GetRagdollID(_id) };
	return ragdollStorage->GetIgnoreFilter(ragdollID);
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

// ウェイト変更の共通処理
bool AnimationComponentAPI::SetActiveRagdollWeightInternal(
	ActiveRagdollID _id, uint32_t _boneIndex,
	float _weight, bool _includeChildren,
	bool _position, bool _rotation)
{
	// 無効チェック
	if (!activeRagdollStorage->IsAlive(_id) || !std::isfinite(_weight))
	{
		return false;
	}

	RagdollID ragdollID{ activeRagdollStorage->GetRagdollID(_id) };
	if (!ragdollStorage->IsAlive(ragdollID))
	{
		return false;
	}

	const Ragdoll& ragdoll{ ragdollStorage->GetRagdoll(ragdollID) };
	if (!skeletonStorage->IsAlive(ragdoll.skeleton))
	{
		return false;
	}

	const SkeletonData* skeleton{ skeletonStorage->GetSkeletonDataPtr(ragdoll.skeleton) };
	if (skeleton == nullptr || _boneIndex >= skeleton->Size())
	{
		return false;
	}

	const ActiveRagdoll& activeRagdoll{ activeRagdollStorage->GetActiveRagdoll(_id) };
	float weight{ std::clamp(_weight, 0.0f, 1.0f) };
	float maxForce{ activeRagdoll.settings.maxJointDriveForce * weight };
	bool updated{ false };

	for (size_t i{ 0 }; i < activeRagdoll.childBoneIndex.size(); ++i)
	{
		uint32_t childBoneIndex{ activeRagdoll.childBoneIndex[i] };
		uint32_t boneIndex{ childBoneIndex };

		if (_includeChildren)
		{
			// 親をたどり、指定したボーン自身または子孫か判定する
			for (size_t depth{ 0 }; depth < skeleton->Size() && boneIndex < skeleton->Size() && boneIndex != _boneIndex; ++depth)
			{
				boneIndex = skeleton->parentIndices[boneIndex];
			}
		}

		if (boneIndex != _boneIndex)
		{
			continue;
		}

		// 各種拘束IDをフラグによって取得したりする
		ConstraintID positionConstraint{
			_position && childBoneIndex < activeRagdoll.pointConstraints.size() ?
			activeRagdoll.pointConstraints[childBoneIndex] :
			ConstraintID{} };
		ConstraintID rotationConstraint{
			_rotation && i < activeRagdoll.jointDriveConstraints.size() ?
			activeRagdoll.jointDriveConstraints[i] :
			ConstraintID{} };
		ConstraintID constraints[]{ positionConstraint, rotationConstraint };

		for (ConstraintID constraint : constraints)
		{
			if (!constraint.IsValid())
			{
				continue;
			}

			ConstraintTuning tuning{ PhysicsComponentAPI::GetTuning(constraint) };
			tuning.maxForce = maxForce;
			PhysicsComponentAPI::SetTuning(constraint, tuning);

			updated = true;
		}
	}

	return updated;
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