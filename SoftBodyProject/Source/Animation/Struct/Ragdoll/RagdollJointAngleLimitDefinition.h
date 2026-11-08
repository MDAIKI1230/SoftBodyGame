#pragma once

union RagdollJointAngleLimitDefinition
{
	struct RagdollSwingTwistLimitDefinition
	{
		// Twist軸からどこまで傾けられるか
		float maxSwingAngleRadians{ 0.0f };

		// Twist軸周りの回転範囲
		float minTwistAngleRadians{ 0.0f };
		float maxTwistAngleRadians{ 0.0f };
	};

	struct RagdollHingeLimitDefinition
	{
		float minAngleRadians{ 0.0f };
		float maxAngleRadians{ 0.0f };
	};

	RagdollSwingTwistLimitDefinition swing;
	RagdollHingeLimitDefinition hinge;

	RagdollJointAngleLimitDefinition() :
		swing{}
	{
	}
};