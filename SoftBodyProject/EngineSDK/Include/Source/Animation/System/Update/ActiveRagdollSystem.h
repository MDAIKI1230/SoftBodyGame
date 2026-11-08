#pragma once

#include "CollisionFilter.h"

#include "SkeletonInstanceStorage.h"
#include "RagdollStorage.h"
#include "ActiveRagdollStorage.h"

/*
	ターゲットポーズに対して、追従するラグドールをここでは実装する。
	状態の遷移などは行わない(外部から変更される予定)。
	状態によって動きは変わる。
*/
class ActiveRagdollSystem
{
public:
	void PrePhysicsFixedUpdate(SkeletonInstanceStorage* _skeletonStorage, RagdollStorage* _ragdollStorage, ActiveRagdollStorage* _activeRagdollStorage);
private:
	// 起き上がるための補助力を加える関数
	void UpdateRecovery(const Ragdoll& _ragdoll, ActiveRagdoll& _activeRagdoll, const CollisionFilter& _ignoreFilter);


	// 腰の拘束の位置更新関数
	void UpdatePelvisConstraint(ActiveRagdoll& _activeRagdoll, const Matrix4x4 _worldFromModel);
	// Animationの目標姿勢から各関節Driveの目標相対回転を更新する関数
	void UpdateJointDrive(const Ragdoll& _ragdoll, const ActiveRagdoll& _activeRagdoll, const PoseBuffer& _targetPose);
	// Animationの目標姿勢から各関節Pointの目標位置を更新する関数
	void UpdatePointConstraint(const Ragdoll& _ragdoll, const ActiveRagdoll& _activeRagdoll, const PoseBuffer& _targetPose, const Matrix4x4 _worldFromModel);
};