#pragma once

#include <string>

#include "MDMath.h"
#include "EntityID.h"
#include "ActiveRagdollID.h"

#include "ActiveRagdollConstants.h"
#include "RagdollConstants.h"

#include "CollisionFilter.h"

#include "RendererComponent.h"
#include "RigidBodyComponent.h"

struct ActiveRagdollComponent
{
	friend class ActiveRagdollComponentStorage;
public:
	// コンストラクタ
	ActiveRagdollComponent(EntityID _entity, const RendererComponent& _rendererComponent, const std::string& _path);

	// --- Body取得 ---

	// 指定した役割のRigidBody取得
	RigidBodyComponent GetBody(RagdollBoneRole _role) const;

	// 名前・役割からボーンインデックス取得
	uint32_t GetBoneIndex(const char* _boneName) const;
	uint32_t GetBoneIndex(RagdollBoneRole _role) const;

	// 指定したボーンのウェイト変更
	bool SetWeight(uint32_t _boneIndex, float _weight);
	bool SetWeight(RagdollBoneRole _role, float _weight);

	// 指定したボーン自身と子孫のウェイト変更
	bool SetBranchWeight(uint32_t _rootBoneIndex, float _weight);
	bool SetBranchWeight(RagdollBoneRole _rootRole, float _weight);

	// --- 操作要求 ---

	// ワールド空間の移動入力取得
	const Vector3& GetMoveInput() const;
	// ワールド空間の移動入力設定
	void SetMoveInput(const Vector3& _moveInput);
	// 移動入力クリア
	void ClearMoveInput();
	// ジャンプ要求
	void RequestJump();

	// --- 状態取得 ---

	// 制御状態取得
	ActiveRagdollControlState GetControlState() const;
	// 接地状態取得
	bool IsGrounded() const;
	// 地面法線取得
	const Vector3& GetGroundNormal() const;
	// 水平速度取得
	const Vector3& GetPlanarVelocity() const;
	// 重心位置取得
	const Vector3& GetCenterOfMass() const;
	// 直立度取得
	float GetUprightDot() const;
	// Ragdoll全体を無視する衝突フィルター取得
	CollisionFilter GetIgnoreFilter() const;
private:
	ActiveRagdollID id;
};