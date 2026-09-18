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

			Quaternion targetRelativeRotation{
				skeleton.targetPose.localRotations[activeRagdoll.parentBoneIndex[i]].Conjugate() *
				skeleton.targetPose.localRotations[activeRagdoll.childBoneIndex[i]] };

			PhysicsComponentAPI::SetTargetRelativeRotation(constraint, targetRelativeRotation);
		}
	}
}