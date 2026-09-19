#include "ResourceManager.h"

#include "PoseMixer.h"

#include "TargetPoseSystem.h"

void TargetPoseSystem::Update(SkeletonInstanceStorage* _skeletonStorage, PoseLayerStorage* _poseLayerStorage)
{
	InitialePose(_skeletonStorage);
	MixPose(_skeletonStorage, _poseLayerStorage);
	ReBuildMatrix(_skeletonStorage);
}

void TargetPoseSystem::InitialePose(SkeletonInstanceStorage* _skeletonStorage)
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

void TargetPoseSystem::MixPose(SkeletonInstanceStorage* _skeletonStorage, PoseLayerStorage* _poseLayerStorage)
{
	for (const PoseLayer& layer : _poseLayerStorage->GetPoseLayerRange())
	{
		SkeletonInstanceData& skeleton{ _skeletonStorage->EditSkeletonInstanceData(layer.skeletonID) };
		PoseMixer::MakeTargetPose(skeleton.targetPose, layer);
	}
}

void TargetPoseSystem::ReBuildMatrix(SkeletonInstanceStorage* _skeletonStorage)
{
	for (SkeletonInstanceData& skeleton : _skeletonStorage->EditSkeletonInstanceDataRange())
	{
		PoseBuffer& targetPose{ skeleton.targetPose };

		for (uint32_t boneIndex{ 0 }; boneIndex < targetPose.Size(); boneIndex++)
		{
			uint32_t parentIndex{ skeleton.skeletonData->parentIndices[boneIndex] };

			// 逆行列も計算
			Matrix4x4 boneFromParent{ MatGenerateFunc::InverseTRS(
				targetPose.localPositions[boneIndex],
				targetPose.localRotations[boneIndex],
				targetPose.localScales[boneIndex]
			) };

			if (parentIndex == UINT32_MAX)
			{
				// モデルからのボーンのローカル姿勢
				targetPose.modelFromBoneMatrices[boneIndex] = targetPose.localMatrices[boneIndex];
				// 上の逆行列
				targetPose.boneFromModelMatrices[boneIndex] = boneFromParent;
			}
			else
			{
				// モデルからのボーンのローカル姿勢
				targetPose.modelFromBoneMatrices[boneIndex] =
					targetPose.modelFromBoneMatrices[parentIndex] * targetPose.localMatrices[boneIndex];

				// 上の逆行列
				targetPose.boneFromModelMatrices[boneIndex] =
					boneFromParent * targetPose.boneFromModelMatrices[parentIndex];
			}
		}
	}
}