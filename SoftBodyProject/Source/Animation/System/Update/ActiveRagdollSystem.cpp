#include "PhysicsComponentAPI.h"
#include "ActiveRagdollSystem.h"

void ActiveRagdollSystem::PrePhysicsFixedUpdate(SkeletonInstanceStorage* _skeletonStorage, RagdollStorage* _ragdollStorage, ActiveRagdollStorage* _activeRagdollStorage)
{
	for (ActiveRagdollID id : _activeRagdollStorage->GetIDRange())
	{
		const ActiveRagdoll& activeRagdoll{ _activeRagdollStorage->GetActiveRagdoll(id) };

		const Ragdoll& ragdoll{ _ragdollStorage->GetRagdoll(_activeRagdollStorage->GetRagdollID(id)) };

		SkeletonInstanceData& skeleton{ _skeletonStorage->EditSkeletonInstanceData(ragdoll.skeleton) };

		for (int i{ 0 }; i < activeRagdoll.jointDriveConstraints.size(); i++)
		{
			/*
				親をオーナーとしている事に注意
			*/

			ConstraintID constraint{ activeRagdoll.jointDriveConstraints[i] };

			// 親ボーンインデックス
			uint32_t parentBoneIndex{ activeRagdoll.parentBoneIndex[i] };
			// 子ボーンインデックス
			uint32_t childBoneIndex{ activeRagdoll.childBoneIndex[i] };

			const EndPointFrame& parentEndPoint{ PhysicsComponentAPI::GetEndPoint(constraint) };
			// 一旦APIの関係上一対一だけどこれからしか取得できないから[0]があってすまぬ
			const EndPointFrame& childEndPoint{ PhysicsComponentAPI::GetOtherEndPoints(constraint)[0] };

			Vector3 pos, scale;
			Quaternion childModelRoatation, parentModelRotation;

			Transform::DecomposeTRS(
				skeleton.targetPose.modelFromBoneMatrices[childBoneIndex],
				pos,
				childModelRoatation,
				scale
			);

			Transform::DecomposeTRS(
				skeleton.targetPose.modelFromBoneMatrices[parentBoneIndex],
				pos,
				parentModelRotation,
				scale
			);

			// それぞれボディ回転に変換
			Quaternion parentBodyRotation{ parentModelRotation * ragdoll.bodyLinks[parentBoneIndex].bodyRotationInBoneSpace };
			Quaternion childBodyRotation{ childModelRoatation * ragdoll.bodyLinks[childBoneIndex].bodyRotationInBoneSpace };

			// 拘束回転に変換(ワールド空間ではないが相対姿勢を求める際には、同じモデル上のため問題ない)
			Quaternion parentJointModel{ parentBodyRotation * parentEndPoint.localRotation };
			Quaternion childJointModel{ childBodyRotation * childEndPoint.localRotation };

			Quaternion targetRelativeRotation{ parentJointModel.Conjugate() * childJointModel };

			targetRelativeRotation.Normalize();

			PhysicsComponentAPI::SetTargetRelativeRotation(constraint, targetRelativeRotation);
		}
	}
}