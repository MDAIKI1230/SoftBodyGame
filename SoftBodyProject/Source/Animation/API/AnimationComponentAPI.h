#pragma once

#include "AnimationWorld.h"
#include "RendererComponent.h"

#include "ActiveRagdollConstants.h"
#include "RagdollConstants.h"
#include "RigidBodyComponent.h"
#include "CollisionFilter.h"

class AnimationComponentAPI
{
public:
	// --- 通常アニメーション ---

	// 生成
	static AnimationID CreateAnimation(EntityID _entity, const RendererComponent* _rendererComponent, const char* _path = nullptr);
	// 破棄
	static void DestroyAnimation(AnimationID _id);

	// アニメーションの名前の取得
	static std::string_view GetAnimationName(AnimationID _id);
	// アニメーションの名前の変更
	static void SetAnimationName(AnimationID _id, const char* _path);

	// アニメーション再生時間取得
	static float GetTime(AnimationID _id);
	// アニメーション再生時間変更
	static void SetTime(AnimationID _id, float _time);
	// アニメーションスピード取得
	static float GetSpeed(AnimationID _id);
	// アニメーションスピード変更
	static void SetSpeed(AnimationID _id, float _speed);

	// ループ再生フラグ取得
	static bool GetLoop(AnimationID _id);
	// ループ再生フラグ変更
	static void SetLoop(AnimationID _id, bool _isLoop);
	// 再生フラグ取得
	static bool GetPlaying(AnimationID _id);
	// 再生フラグ変更
	static void SetPlaying(AnimationID _id, bool _isPlay);

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

	// --- Body取得 ---

	// 指定した役割のRigidBody取得
	static RigidBodyComponent GetActiveRagdollBody(ActiveRagdollID _id, RagdollBoneRole _role);

	// --- 操作要求 ---

	// ワールド空間の移動入力取得
	static const Vector3& GetActiveRagdollMoveInput(ActiveRagdollID _id);
	// ワールド空間の移動入力設定
	static void SetActiveRagdollMoveInput(ActiveRagdollID _id, const Vector3& _moveInput);
	// ジャンプ要求
	static void RequestActiveRagdollJump(ActiveRagdollID _id);

	// --- 状態取得 ---

	// 制御状態取得
	static ActiveRagdollControlState GetActiveRagdollControlState(ActiveRagdollID _id);
	// 接地状態取得
	static bool GetActiveRagdollGrounded(ActiveRagdollID _id);
	// 地面法線取得
	static const Vector3& GetActiveRagdollGroundNormal(ActiveRagdollID _id);
	// 水平速度取得
	static const Vector3& GetActiveRagdollPlanarVelocity(ActiveRagdollID _id);
	// 重心位置取得
	static const Vector3& GetActiveRagdollCenterOfMass(ActiveRagdollID _id);
	// 直立度取得
	static float GetActiveRagdollUprightDot(ActiveRagdollID _id);

	// Ragdoll全体を無視する衝突フィルター取得
	static CollisionFilter GetActiveRagdollIgnoreFilter(ActiveRagdollID _id);

	// --- IK ---

	// ハンドIK作成
	static FeatureIKID CreateHandIK(EntityID _entity, const RendererComponent* _rendererComponent);
	// IK破棄
	static void DestroyIK(FeatureIKID _id);

	// 上腕ボーン取得
	static std::string_view GetUpperArm(FeatureIKID _id);
	// 上腕ボーン変更
	static void SetUpperArm(FeatureIKID _id, const char* _boneName);

	// 前腕ボーン取得
	static std::string_view GetLowerArm(FeatureIKID _id);
	// 前腕ボーン変更
	static void SetLowerArm(FeatureIKID _id, const char* _boneName);

	// 手ボーン取得
	static std::string_view GetHand(FeatureIKID _id);
	// 手ボーン変更
	static void SetHand(FeatureIKID _id, const char* _boneName);

	// ターゲット位置取得
	static Vector3 GetTargetPosition(FeatureIKID _id);
	// ターゲット位置変更
	static void SetTargetPosition(FeatureIKID _id, const Vector3& _targetPosition);

	// ターゲット回転取得
	static Quaternion GetTargetRotation(FeatureIKID _id);
	// ターゲット回転変更
	static void SetTargetRotation(FeatureIKID _id, const Quaternion& _targetRotation);

	// ポール位置取得
	static Vector3 GetPolePosition(FeatureIKID _id);
	// ポール位置変更
	static void SetPolePosition(FeatureIKID _id, const Vector3& _polePosition);

	// 位置ウェイト取得
	static float GetPositionWeight(FeatureIKID _id);
	// 位置ウェイト変更
	static void SetPositionWeight(FeatureIKID _id, float _positionWeight);

	// 回転ウェイト取得
	static float GetRotationWeight(FeatureIKID _id);
	// 回転ウェイト変更
	static void SetRotationWeight(FeatureIKID _id, float _rotationWeight);

	static void BindWorld(AnimationWorld* _world);
	static void UnbindWorld();
private:
	AnimationComponentAPI() = default;

	// 名前からボーンインデックス取得関数
	static uint32_t FindHandBoneIndex(FeatureIKID _id, const char* _boneName);
	// ボーンインデックスから名前取得
	static std::string_view GetHandBoneName(FeatureIKID _id, uint32_t _boneIndex);
private:
	static SkeletonInstanceStorage* skeletonStorage;
	static AnimationStorage* animationStorage;
	static PoseLayerStorage* poseLayerStorage;
	static RagdollStorage* ragdollStorage;
	static ActiveRagdollStorage* activeRagdollStorage;
	static SolverIKStorage* solverIKStorage;
	static FeatureIKStorage* featureIKStorage;
};
