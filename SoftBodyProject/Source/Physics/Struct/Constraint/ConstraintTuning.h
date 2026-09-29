#pragma once

#include <cfloat>

struct ConstraintTuning
{
	// 拘束の違反の許容値(0で固い拘束)
	float compliance{ 0.0f };
    // 柔らかさ
    float stiffness{ 10000.0f };
    // 減衰係数
    float damping{ 1000.0f };
    // λの最大強さ
    float maxForce{ FLT_MAX };
};
