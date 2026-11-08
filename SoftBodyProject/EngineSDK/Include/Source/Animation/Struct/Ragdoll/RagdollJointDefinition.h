#pragma once

#include <string>

#include "MDMath.h"

#include "ConstraintConstants.h"

#include "RagdollJointAngleLimitDefinition.h"

/*
	ラグドール生成時のボーンの繋がりの定義構造体
*/
struct RagdollJointDefinition
{
	// 親ボーンの名前
	std::string parentBoneName;

	// 拘束位置
	Vector3 constraintPositionLocalChild;
	// 拘束姿勢
	Quaternion constraintRotationLocalChild;

	// 拘束種類
	ConstraintType type{ ConstraintType::ANGLE_LIMIT_HINGE };

	// 拘束の制限角度
	RagdollJointAngleLimitDefinition angleLimit;
};