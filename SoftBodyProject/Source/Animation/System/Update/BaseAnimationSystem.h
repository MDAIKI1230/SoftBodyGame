#pragma once

#include "AnimationStorage.h"
#include "PoseLayerStorage.h"

/*
	ただただアニメーションを再生させるシステム
	将来的には、アニメーションの変更時にきれいに推移するのとかも入れる。
	今は、ただ指定されたアニメーションの再生だけでOK
*/
class BaseAnimationSystem
{
public:
	void Update(AnimationStorage* _animationStorage, PoseLayerStorage* _poseLayerStorage);
};