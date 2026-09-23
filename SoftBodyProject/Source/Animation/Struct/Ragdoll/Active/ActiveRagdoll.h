#pragma once

#include <vector>
#include <cstdint>

#include "ConstraintID.h"

#include "ActiveRagdollSetting.h"
#include "FootGroundInfo.h"

/*
	Ragdollのボディ系の情報もBoneIndexに対応していることを利用する。
*/
struct ActiveRagdoll
{
public:
	void ResetTime()
	{
		standingTime = 0.0f;
		airborneTime = 0.0f;
		fallenTime = 0.0f;
		jumpTime = 0.0f;
	}
public:
	// 子のボーンインデックス
	std::vector<uint32_t> childBoneIndex;
	// 親のボーンインデックス
	std::vector<uint32_t> parentBoneIndex;
	// 二者間に起きる拘束のID
	std::vector<ConstraintID> jointDriveConstraints;

	// 操作目標
	Vector3 moveInput{};
	bool jumpRequested{};

	// 接地状態
	FootGroundInfo leftFootGround;
	FootGroundInfo rightFootGround;
	bool isGrounded{};
	Vector3 groundNormal{ Vector3::UP };

	// 身体状態
	Vector3 planarVelocity{};
	Vector3 centerOfMass{};
	float uprightDot{ 1.0f };

	// 制御状態
	ActiveRagdollControlState controlState{ ActiveRagdollControlState::AIRBORNE };
	float standingTime{ 0.0f };
	float airborneTime{ 0.0f };
	float fallenTime{ 0.0f };
	float jumpTime{ 0.0f };

	// 調整値
	ActiveRagdollSetting settings;
};