#include "ResourceManager.h"
#include "TimeManager.h"

#include "BaseAnimationSystem.h"

void BaseAnimationSystem::Update(AnimationStorage* _animationStorage, PoseLayerStorage* _poseLayerStorage)
{
	for (auto& animation : _animationStorage->EditAnimationInstanceDataRange())
	{
		animation.time += TimeManager::GetFixedDeltaTime() * animation.speed;

		AnimationHandle animationHandle{
			ResourceManager::AttachAnimation(
			animation.modelHandle,
			animation.animationName) };

		ResourceManager::SetAnimationTime(
			animation.modelHandle,
			animationHandle,
			animation.time);

		ResourceManager::GetPose(
			animation.modelHandle,
			_poseLayerStorage->EditPoseLayer(animation.layerID).pose);
	}
}