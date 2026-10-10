#pragma once

#include "Base/ConstraintBase.h"
#include "ConstraintTuning.h"

/*
	コンポーネントの所有者自身が基盤となる軸を持ち
	追加された奴らは、その軸を元に拘束をされる
*/
struct HingeConstraint : public ConstraintBase
{
	// 柔らかさなどの調整用数値(角度と位置で別に用意)
	ConstraintTuning positionTuning;
	ConstraintTuning angularTuning;
};
