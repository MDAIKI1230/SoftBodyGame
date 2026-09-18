#pragma once

#include <string>

#include "ModelHandle.h"

#include "PoseLayerID.h"

/*
	実行時のアニメーションデータ
*/
struct AnimationInstanceData
{
	ModelHandle modelHandle;
	// アニメーションの名前
	std::string animationName;

	PoseLayerID layerID;

	float time{ 0.0f };
	float speed{ 1.0f };

	bool loop{ true };
	bool playing{ true };
};