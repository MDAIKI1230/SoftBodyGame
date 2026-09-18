#pragma once

#include <vector>
#include <cstdint>

#include "ConstraintID.h"

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
	std::vector<ConstraintID> jointDriveConstraints;
};