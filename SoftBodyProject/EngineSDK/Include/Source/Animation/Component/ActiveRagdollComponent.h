#pragma once

#include <string>

#include "MDMath.h"
#include "EntityID.h"
#include "ActiveRagdollID.h"

#include "ActiveRagdollConstants.h"
#include "RagdollConstants.h"

#include "CollisionFilter.h"

#include "ModelRendererComponent.h"
#include "RigidBodyComponent.h"
#include "ColliderComponent.h"

struct ActiveRagdollComponent
{
	friend class ActiveRagdollComponentStorage;
public:
	// コンストラクタ
	ActiveRagdollComponent(EntityID _entity, const ModelRendererComponent& _rendererComponent, const std::string& _path);

	// --- Body取得 ---

	// 指定した役割のRigidBody取得
	RigidBodyComponent GetBody(RagdollBoneRole _role) const;

	// 指定したボーンのCollider取得
	ColliderComponent GetCollider(uint32_t _boneIndex) const;
	// 指定した役割のCollider取得
	ColliderComponent GetCollider(RagdollBoneRole _role) const;

	// 名前・役割からボーンインデックス取得
	uint32_t GetBoneIndex(const char* _boneName) const;
	uint32_t GetBoneIndex(RagdollBoneRole _role) const;

	// 指定したボーンのウェイト変更
	bool SetWeight(uint32_t _boneIndex, float _weight);
	bool SetWeight(RagdollBoneRole _role, float _weight);

	// 指定したボーン自身と子孫のウェイト変更
	bool SetBranchWeight(uint32_t _rootBoneIndex, float _weight);
	bool SetBranchWeight(RagdollBoneRole _rootRole, float _weight);

	// 指定したボーンの位置拘束の強さを変更
	bool SetPositionWeight(uint32_t _boneIndex, float _weight);
	bool SetPositionWeight(RagdollBoneRole _role, float _weight);

	// 指定したボーンの回転拘束の強さを変更
	bool SetRotationWeight(uint32_t _boneIndex, float _weight);
	bool SetRotationWeight(RagdollBoneRole _role, float _weight);

	// 指定したボーン自身と子孫の位置拘束の強さを変更
	bool SetBranchPositionWeight(uint32_t _rootBoneIndex, float _weight);
	bool SetBranchPositionWeight(RagdollBoneRole _rootRole, float _weight);

	// 指定したボーン自身と子孫の回転拘束の強さを変更
	bool SetBranchRotationWeight(uint32_t _rootBoneIndex, float _weight);
	bool SetBranchRotationWeight(RagdollBoneRole _rootRole, float _weight);

	// 指定したボーンの空中追従倍率を変更。倍率は0～1へ制限する
	bool SetAirborneFollowScale(uint32_t _boneIndex, float _scale);
	bool SetAirborneFollowScale(RagdollBoneRole _role, float _scale);

	// ワールド空間の目標位置を設定。関連するボーンのWeightは呼び出し側で調整する
	bool SetTargetPosition(uint32_t _boneIndex, const Vector3& _worldPosition);
	// ワールド空間の目標位置を設定。関連するボーンのWeightは呼び出し側で調整する
	bool SetTargetPosition(RagdollBoneRole _role, const Vector3& _worldPosition);

	// 位置指定を解除
	bool ClearTargetPosition(uint32_t _boneIndex);
	// 位置指定を解除
	bool ClearTargetPosition(RagdollBoneRole _role);

	// 現在のボーンのワールド位置を取得
	bool GetBoneWorldPosition(uint32_t _boneIndex, Vector3& _worldPosition) const;
	bool GetBoneWorldPosition(RagdollBoneRole _role, Vector3& _worldPosition) const;

	// --- 状態 ---

	// 制御状態取得
	ActiveRagdollControlState GetControlState() const;
	// 制御状態変更
	ActiveRagdollControlState SetControlState(ActiveRagdollControlState _state);
	// Ragdoll全体を無視する衝突フィルター取得
	CollisionFilter GetIgnoreFilter() const;
private:
	ActiveRagdollID id;
};