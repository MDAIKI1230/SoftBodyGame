#pragma once

#include <string>

#include "ModelHandle.h"
#include "AnimationHandle.h"

#include "PoseLayerID.h"

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

	float time{ 0.0f };
	float speed{ 1.0f };
	float total{ 0.0f };

	bool loop{ false };
	bool playing{ false };
};