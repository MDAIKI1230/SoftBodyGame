#include "ResourceManager.h"

#include "TargetPoseSystem.h"

void TargetPoseSystem::Update(SkeletonInstanceStorage* _skeletonStorage)
{
	for (auto& skeleton : _skeletonStorage->EditSkeletonInstanceDataRange())
	{
		PoseBuffer& targetPose{ skeleton.targetPose };

		// 一旦初期ポーズを保ち続けるのを目標に作るべし
		for (int boneIndex{ 0 }; boneIndex < targetPose.localMatrices.size(); boneIndex++)
		{
			targetPose.localPositions[boneIndex] = skeleton.skeletonData->bindLocalPositions[boneIndex];
			targetPose.localRotations[boneIndex] = skeleton.skeletonData->bindLocalRotations[boneIndex];
			targetPose.localScales[boneIndex] = skeleton.skeletonData->bindLocalScales[boneIndex];

			targetPose.localMatrices[boneIndex] = skeleton.skeletonData->bindLocalMatrices[boneIndex];
		}
	}
}