#pragma once

#include "PoseLayer.h"

/*
	PoseLayerの情報をブレンドして新たなポーズを作る関数を置く場所
	TargetPoseSystemから使われる
*/
namespace PoseMixer
{
	void MakeTargetPose(PoseBuffer& _targetPose, const PoseLayer& _layer);
}