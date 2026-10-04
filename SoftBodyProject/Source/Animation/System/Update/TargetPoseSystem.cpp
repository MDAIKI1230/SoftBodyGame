#include "ResourceManager.h"

#include "PoseMixer.h"

#include "TargetPoseSystem.h"

void TargetPoseSystem::Update(SkeletonInstanceStorage* _skeletonStorage, PoseLayerStorage* _poseLayerStorage)
{
	InitialePose(_skeletonStorage);
	MixPose(_skeletonStorage, _poseLayerStorage);
	ReBuildMatrix(_skeletonStorage);
	ApplyPositionTarget(_skeletonStorage);
	ReBuildMatrix(_skeletonStorage);
}

void TargetPoseSystem::InitialePose(SkeletonInstanceStorage* _skeletonStorage)
{
	for (SkeletonID id : _skeletonStorage->GetIDRange())
	{
		PoseBuffer& targetPose{ _skeletonStorage->EditTargetPose(id) };

		const SkeletonData* skeletonData{ _skeletonStorage->GetSkeletonDataPtr(id) };

		if (skeletonData == nullptr)
		{
			continue;
		}

		// 一旦初期ポーズを保ち続けるのを目標に作るべし
		for (int boneIndex{ 0 }; boneIndex < targetPose.localMatrices.size(); boneIndex++)
		{
			targetPose.localPositions[boneIndex] = skeletonData->bindLocalPositions[boneIndex];
			targetPose.localRotations[boneIndex] = skeletonData->bindLocalRotations[boneIndex];
			targetPose.localScales[boneIndex] = skeletonData->bindLocalScales[boneIndex];

			targetPose.localMatrices[boneIndex] = skeletonData->bindLocalMatrices[boneIndex];
		}
	}
}

void TargetPoseSystem::MixPose(SkeletonInstanceStorage* _skeletonStorage, PoseLayerStorage* _poseLayerStorage)
{
	for (const PoseLayer& layer : _poseLayerStorage->GetPoseLayerRange())
	{
		PoseMixer::MakeTargetPose(_skeletonStorage->EditTargetPose(layer.skeletonID), layer);
	}
}

void TargetPoseSystem::ApplyPositionTarget(SkeletonInstanceStorage* _skeletonStorage)
{
	for (auto id : _skeletonStorage->GetIDRange())
	{
		auto& positionTargets{ _skeletonStorage->EditPositionTargets(id) };

		// コンテナ内に何もないなら飛ばす
		if (positionTargets.empty())
		{
			continue;
		}

		PoseBuffer& targetPose{ _skeletonStorage->EditTargetPose(id) };
		const Matrix4x4& worldFromModel{ _skeletonStorage->GetWorldFromModel(id)};
		const SkeletonData* skeleton{ _skeletonStorage->GetSkeletonDataPtr(id) };

		Vector3 pos, scale;
		Quaternion rot;

		Transform::DecomposeTRS(
			worldFromModel,
			pos,
			rot,
			scale
		);

		Matrix4x4 modelFromWorld{ MatGenerateFunc::InverseTRS(
			pos,
			rot,
			scale
		) };

		for (const PositionTarget& target : positionTargets)
		{
			uint32_t boneIndex{ target.boneIndex };
			uint32_t parentIndex{ skeleton->parentIndices[boneIndex] };
			Vector3 modelPos{ modelFromWorld * target.worldPosition };
			Vector3 localPos{
				parentIndex != UINT32_MAX ?
					targetPose.boneFromModelMatrices[parentIndex] * modelPos :
					modelPos };
			targetPose.localPositions[boneIndex] = localPos;
		}
	}
}

void TargetPoseSystem::ReBuildMatrix(SkeletonInstanceStorage* _skeletonStorage)
{
	for (SkeletonID id : _skeletonStorage->GetIDRange())
	{
		PoseBuffer& targetPose{ _skeletonStorage->EditTargetPose(id) };

		const SkeletonData* skeletonData{ _skeletonStorage->GetSkeletonDataPtr(id) };

		if (skeletonData == nullptr)
		{
			continue;
		}

		// ローカル行列も計算しとこう。
		for (uint32_t boneIndex{ 0 }; boneIndex < targetPose.Size(); boneIndex++)
		{
			targetPose.localMatrices[boneIndex] = MatGenerateFunc::TRS(
				targetPose.localPositions[boneIndex],
				targetPose.localRotations[boneIndex],
				targetPose.localScales[boneIndex]
			);
		}

		for (uint32_t boneIndex{ 0 }; boneIndex < targetPose.Size(); boneIndex++)
		{
			uint32_t parentIndex{ skeletonData->parentIndices[boneIndex]};

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