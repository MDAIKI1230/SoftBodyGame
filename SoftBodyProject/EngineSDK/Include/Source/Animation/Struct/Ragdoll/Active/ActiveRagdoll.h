#pragma once

#include <vector>
#include <cstdint>

#include "ActiveRagdollConstants.h"

#include "ConstraintID.h"

#include "ActiveRagdollSetting.h"

/*
	Ragdollのボディ系の情報もBoneIndexに対応していることを利用する。
*/
struct ActiveRagdoll
{
	// 子のボーンインデックス
	std::vector<uint32_t> childBoneIndex;
	// 親のボーンインデックス
	std::vector<uint32_t> parentBoneIndex;

	// 二者間に起きる拘束のID
	// 回転の拘束ID
	std::vector<ConstraintID> rotationConstraints;
	// 回転の重み
	BoneMask rotationWeights;
	// 位置の拘束ID
	std::vector<ConstraintID> positionConstraints;
	// 位置の重み
	BoneMask positionWeights;

	// 位置拘束用のTransformID
	std::vector<PhysicsTransformID> pointTransformIDs;
	// 腰の縦方向が自由な拘束のID
	ConstraintID pelvisConstraintID;
	// 腰の拘束操作用TransformID
	PhysicsTransformID pelvisConstraintTransformID;

	// 制御状態
	ActiveRagdollControlState controlState{ ActiveRagdollControlState::AIRBORNE };

	// 調整値
	ActiveRagdollSetting settings;
};