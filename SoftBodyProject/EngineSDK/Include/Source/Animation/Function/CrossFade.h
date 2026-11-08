#pragma once

#include "AnimationConstants.h"

#include "Buffer/PoseBuffer.h"

#include "ModelHandle.h"
#include "AnimationHandle.h"

class CrossFade
{
public:
	CrossFade(ModelHandle _model);

	AnimationState operator()(PoseBuffer& _output, float _deltaTime);

	// 遷移にかける時間が負値の場合1秒になるため注意
	void SetUp(AnimationHandle _startAnimation, AnimationHandle _endAnimation, float _transitionDuration, float _startTransitionTime);
private:
	bool SetPoseBuffer(PoseBuffer& _pose, AnimationHandle _animation, float _playTime);
private:
	ModelHandle model;
	AnimationHandle startAnimation;
	AnimationHandle endAnimation;
	// 遷移にかける時間
	float transitionDuration{ 1.0f };
	// 遷移開始時点のアニメーション時間
	float startTransitionTime{ 0.0f };
	// 経過時間
	float transitionElapsedTime{ 0.0f };
};