#include "PhysicsComponentAPI.h"

#include "RagdollSystem.h"

// Physics実行前
void RagdollSystem::PrePhysicsFixedUpdate(SkeletonInstanceStorage* _skeletonStorage, RagdollStorage* _ragdollStorage)
{
	for (auto& id : _ragdollStorage->GetIDRange())
	{
		if (!_ragdollStorage->GetNeedInitialize(id))
		{
			continue;
		}

		// Animation Pose → Physics Body
		SkeletonID skeletonID{ _ragdollStorage->EditRagdoll(id).skeleton };

		SkeletonInstanceData& skeleton{ _skeletonStorage->EditSkeletonInstanceData(skeletonID) };

		// ModelのすべてのBoneの親ボーンのローカルに変換してった行列を入れていくところ。(命名が違うが意味は一緒)
		std::vector<Matrix4x4>& modelMatrices{ skeleton.outputPose.modelFromBoneMatrices };

		// 全BoneのModel行列を作る
		for (uint32_t bone{ 0 }; bone < skeleton.outputPose.localPositions.size(); bone++)
		{
			Matrix4x4 localMatrix{ MatGenerateFunc::TRS(
				skeleton.outputPose.localPositions[bone],
				skeleton.outputPose.localRotations[bone],
				skeleton.outputPose.localScales[bone]) };

			uint32_t parent{ skeleton.skeletonData->parentIndices[bone] };

			modelMatrices[bone] =
				parent == UINT32_MAX
				? localMatrix
				: modelMatrices[parent] * localMatrix;
		}

		// 全Boneをワールド姿勢に変換し、それをBodyに反映する
		for (const RagdollBodyLink& link : _ragdollStorage->GetRagdoll(id).bodyLinks)
		{
			Matrix4x4 boneFromBody{ MatGenerateFunc::TRS(
				link.bodyPositionInBoneSpace,
				link.bodyRotationInBoneSpace,
				Vector3::ONE) };

			Matrix4x4 worldFromBody{
				_skeletonStorage->GetWorldFromModel(skeletonID) *
				modelMatrices[link.boneIndex] *
				boneFromBody };

			Vector3 position;
			Quaternion rotation;
			Vector3 scale;
			Transform::DecomposeTRS(worldFromBody, position, rotation, scale);

			PhysicsComponentAPI::SetRigidBodyPosition(link.bodyID, position);
			PhysicsComponentAPI::SetRigidBodyRoatation(link.bodyID, rotation);
		}

		_ragdollStorage->SetNeedInitialize(id, false);
	}
}

// Physics実行後
void RagdollSystem::PostPhysicsFixedUpdate(SkeletonInstanceStorage* _skeletonStorage, RagdollStorage* _ragdollStorage)
{
	for (auto& ragdoll : _ragdollStorage->EditRagdollRange())
	{
		// Physics Body → Animation Pose

		SkeletonInstanceData& skeleton{ _skeletonStorage->EditSkeletonInstanceData(ragdoll.skeleton) };

		// Ragdoll対象外Boneを残す
		skeleton.outputPose = skeleton.targetPose;

		//for (uint32_t bone{ 0 }; bone < skeleton.outputPose.localPositions.size(); bone++)
		//{
		//	if (BoneにBodyが対応している)
		//	{
		//		// PhysicsからBody Worldを取得
		//		// Bone Worldへ変換
		//		// Model Localへ変換
		//		// 親Bone Localへ変換
		//		// outputPoseへTRSを書き込む
		//	}
		//	else
		//	{
		//		// targetPoseのLocalを使ってModel行列だけ更新
		//	}
		//}
	}
}