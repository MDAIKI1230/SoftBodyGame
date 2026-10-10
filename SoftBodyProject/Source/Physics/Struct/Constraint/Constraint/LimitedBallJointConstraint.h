#pragma once

#include "Base/ConstraintBase.h"
#include "ConstraintTuning.h"

/*
	角度付きの点拘束に、軸に対しての角度制限もある。
*/
struct LimitedBallJointConstraint : public ConstraintBase
{
	// Twist制限角度
	float twistAngleMax{ 0.0f };
	float twistAngleMin{ 0.0f };
	// Swing制限角度
	float swingAngle{ 0.0f };

	// 柔らかさなどの調整用数値(角度と位置で別に用意)
	ConstraintTuning positionTuning;
	ConstraintTuning angularTuning;
};