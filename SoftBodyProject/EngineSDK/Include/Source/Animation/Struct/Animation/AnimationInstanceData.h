#pragma once

#include <string>

#include "ModelHandle.h"
#include "AnimationHandle.h"

#include "PoseLayerID.h"

#include "AnimationConstants.h"
#include "CrossFade.h"

/*
	実行時のアニメーションデータ
*/
struct AnimationInstanceData
{
	ModelHandle modelHandle;
	// アニメーションハンドル
	AnimationHandle animationHandle;
	// アニメーションの名前
	std::string animationName;

	PoseLayerID layerID;

	CrossFade fade{ modelHandle };
	AnimationState state{ AnimationState::END };

	float time{ 0.0f };
	float speed{ 1.0f };
	float total{ 0.0f };

	bool loop{ false };
};