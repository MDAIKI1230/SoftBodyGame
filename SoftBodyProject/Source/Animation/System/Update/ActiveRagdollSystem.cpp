#include "PhysicsComponentAPI.h"
#include "ActiveRagdollSystem.h"

void ActiveRagdollSystem::PrePhysicsFixedUpdate(SkeletonInstanceStorage* _skeletonStorage, RagdollStorage* _ragdollStorage, ActiveRagdollStorage* _activeRagdollStorage)
{
	for (ActiveRagdollID id : _activeRagdollStorage->GetIDRange())
	{
		const ActiveRagdoll& activeRagdoll{ _activeRagdollStorage->GetActiveRagdoll(id) };

		const Ragdoll& ragdoll{ _ragdollStorage->GetRagdoll(_activeRagdollStorage->GetRagdollID(id)) };

		const SkeletonInstanceData& skeleton{ _skeletonStorage->GetSkeletonInstanceData(ragdoll.skeleton) };

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

			
			const Quaternion& parentBodyRotation{ skeleton.targetPose.localRotations[parentBoneIndex] };
			const Quaternion& childBodyRotation{ skeleton.targetPose.localRotations[childBoneIndex] };

			Quaternion parentJointWorld{ parentBodyRotation * parentEndPoint.localRotation };

			Quaternion childJointWorld{ childBodyRotation * childEndPoint.localRotation };

			Quaternion targetRelativeRotation{ parentJointWorld.Conjugate() * childJointWorld };

			PhysicsComponentAPI::SetTargetRelativeRotation(constraint, Quaternion::IDENTITY);
		}
	}
}