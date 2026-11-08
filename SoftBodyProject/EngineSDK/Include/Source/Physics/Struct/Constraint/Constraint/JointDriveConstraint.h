#pragma once

#include "Base/ConstraintOneBase.h"
#include "ConstraintTuning.h"
/*
	関節駆動拘束
	 ・関節を動かしたい方向に動かすための拘束
	 ・二つのオブジェクト間の角度を決められた角度に拘束する場合に使う
*/
struct JointDriveConstraint : public ConstraintOneBase
{
	// 拘束相対姿勢
	Quaternion targetRelativeRotation;

	// 柔らかさなどの調整用数値
	ConstraintTuning tuning;
};