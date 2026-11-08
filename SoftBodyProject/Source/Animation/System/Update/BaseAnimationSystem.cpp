#include "ResourceManager.h"
#include "TimeManager.h"

#include "BaseAnimationSystem.h"

void BaseAnimationSystem::Update(AnimationStorage* _animationStorage, PoseLayerStorage* _poseLayerStorage)
{
	for (auto& animation : _animationStorage->EditAnimationInstanceDataRange())
	{
		if (!animation.animationHandle.IsValid())
		{
			continue;
		}

		if (animation.playing == false)
		{
			continue;
		}

		animation.time += TimeManager::GetDeltaTime() * animation.speed;

		if (animation.loop)
		{
			if (animation.time >= animation.total)
			{
				animation.time = 0;
			}
		}

		ResourceManager::SetAnimationTime(
			animation.modelHandle,
			animation.animationHandle,
			animation.time);

		ResourceManager::GetPose(
			animation.modelHandle,
			_poseLayerStorage->EditPoseLayer(animation.layerID).pose);
	}
}

// デバッグ用
void BaseAnimationSystem::PreRenderUpdate(SkeletonInstanceStorage* _skeletonStorage)
{
	for (SkeletonID id : _skeletonStorage->GetIDRange())
	{
		_skeletonStorage->EditOutputPose(id) = _skeletonStorage->EditTargetPose(id);
	}
}