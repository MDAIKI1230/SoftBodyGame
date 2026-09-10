#pragma once

#include <cstdint>

enum class RagdollBoneRole :uint8_t
{
	// 骨盤
	PELVIS,
	// 胴体
	TORSO,
	// 頭
	HEAD,

	// 左腕の上部
	LEFT_UPPER_ARM,
	// 左腕の下部
	LEFT_LOWER_ARM,
	// 左手
	LEFT_HAND,

	// 右腕の上部
	RIGHT_UPPER_ARM,
	// 右腕の下部
	RIGHT_LOWER_ARM,
	// 右手
	RIGHT_HAND,

	// 左太もも
	LEFT_UPPER_LEG,
	// 左脚
	LEFT_LOWER_LEG,
	// 左足
	LEFT_FOOT,

	// 右太もも
	RIGHT_UPPER_LEG,
	// 右脚
	RIGHT_LOWER_LEG,
	// 右足
	RIGHT_FOOT,

	COUNT
};