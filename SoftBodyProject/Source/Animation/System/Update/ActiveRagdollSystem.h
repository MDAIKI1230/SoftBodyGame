#pragma once

#include "CollisionFilter.h"

#include "SkeletonInstanceStorage.h"
#include "RagdollStorage.h"
#include "ActiveRagdollStorage.h"

/*
	今日は一旦スケルトンインスタンスデータのターゲットポーズを取るように動く拘束を発生させる感じ
	各関節駆動拘束に対して正しい角度を入れてあげるのが今日までの仕事
*/
class ActiveRagdollSystem
{
public:
	void PrePhysicsFixedUpdate(SkeletonInstanceStorage* _skeletonStorage, RagdollStorage* _ragdollStorage, ActiveRagdollStorage* _activeRagdollStorage);
private:
	// 左右の足情報からRagdoll全体の接地状態を更新する関数
	void UpdateGroundState(const Ragdoll& _ragdoll, ActiveRagdoll& _activeRagdoll, const CollisionFilter& _ignoreFilter);
	// 指定した足の接地点・地面法線・傾斜状態を更新する関数
	void UpdateFootGroundState(
		const Ragdoll& _ragdoll, FootGroundInfo& _footGroundInfo,
		const ActiveRagdollSetting& _setting, const CollisionFilter& _ignoreFilter, uint32_t _boneIndex);

	// 腰の水平速度・胴体の傾き・重心などの現在状態を更新する関数
	void UpdateBodyState(const Ragdoll& _ragdoll, ActiveRagdoll& _activeRagdoll);
	// 接地状態や胴体の傾きから立位・空中・転倒状態を更新する関数
	void UpdateControlState(ActiveRagdoll& _activeRagdoll);

	// 立ち上がるための力を加える関数
	void UpdateRecovery(const Ragdoll& _ragdoll, ActiveRagdoll& _activeRagdoll);
	// ジャンプリクエストを処理し、ジャンプの力を加える関数
	void UpdateJump(const Ragdoll& _ragdoll, ActiveRagdoll& _activeRagdoll);
	// 入力から目標速度を作り、移動または減速用のForceを加える関数
	void UpdateMovement(const Ragdoll& _ragdoll, ActiveRagdoll& _activeRagdoll);
	// 腰と胸へ姿勢を直立させるTorqueを加える関数
	void UpdateUpright(const Ragdoll& _ragdoll, ActiveRagdoll& _activeRagdoll);
	// 重心と足の支持範囲から姿勢を安定させる補助Forceを加える関数
	void UpdateBalance(const Ragdoll& _ragdoll, ActiveRagdoll& _activeRagdoll);

	// Animationの目標姿勢から各関節Driveの目標相対回転を更新する関数
	void UpdateJointDrive(const Ragdoll& _ragdoll, const ActiveRagdoll& _activeRagdoll, const PoseBuffer& _targetPose);

	// 腰の速度から地面法線方向の成分を除いた水平速度を計算する関数
	Vector3 CalculatePlanarVelocity(const ActiveRagdoll& _activeRagdoll, BodyID _pelvisBodyID) const;
	// 各Bodyの位置と質量からRagdoll全体の重心を計算する関数
	Vector3 CalculateCenterOfMass(const Ragdoll& _ragdoll) const;
	// 胴体の傾きや制御状態から移動操作へ掛ける倍率を計算する関数
	float CalculateMovementControlScale(const ActiveRagdoll& _activeRagdoll) const;
	// 腰/胸の直立させるためのトルク計算関数
	Vector3 CalculateUprightTorque(const ActiveRagdoll& _activeRagdoll, BodyID _bodyID, float _strength) const;
};