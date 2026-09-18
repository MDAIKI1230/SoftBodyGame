#include "PhysicsComponentAPI.h"
#include "AnimationComponentAPI.h"

#include "ActiveRagdollStorage.h"

// 作成関数
ActiveRagdollID ActiveRagdollStorage::Create(EntityID _entity, RagdollID _ragdollID, const Ragdoll& _ragdoll, const SkeletonInstanceData& _skeleton, const std::string& _path)
{
	ActiveRagdollID id{ CreateID(CountRagdollID()) };

	// 失敗した場合は無効値を返す
	if (!CreateActiveRagdoll(_entity, _ragdoll, _skeleton))
	{
		return {};
	}

	ragdollIDs.push_back(_ragdollID);
	ids.push_back(id);
	ownerEntities.push_back(_entity);

	return id;
}

// 破棄
void ActiveRagdollStorage::Destroy(ActiveRagdollID _id)
{
	// 生存確認
	if (!IsAlive(_id))
	{
		return;
	}

	uint32_t denseIndex{ GetDenseIndex(_id) };
	uint32_t lastIndex{ static_cast<uint32_t>(ids.size() - 1) };

	DestroyActiveRagdoll(ragdollIDs[denseIndex], activeRagdolls[denseIndex]);

	// Swap＆Pop
	if (denseIndex != lastIndex)
	{
		ActiveRagdollID movedID{ ids[lastIndex] };

		ragdollIDs[denseIndex] = std::move(ragdollIDs[lastIndex]);

		activeRagdolls[denseIndex] = std::move(activeRagdolls[lastIndex]);

		ownerEntities[denseIndex] = ownerEntities[lastIndex];

		ids[denseIndex] = movedID;

		// 移動したInstanceの参照先を更新
		EditDenseIndex(movedID) = denseIndex;
	}

	ragdollIDs.pop_back();
	activeRagdolls.pop_back();
	ownerEntities.pop_back();
	ids.pop_back();

	ReleaseID(_id);
}

// ActiveRagdoll情報の作成関数
bool ActiveRagdollStorage::CreateActiveRagdoll(EntityID _entity, const Ragdoll& _ragdoll, const SkeletonInstanceData& _skeleton)
{
	const std::vector<uint32_t>& parentIndices{ _skeleton.skeletonData->parentIndices };

	for (int boneIndex{ 0 }; boneIndex < _ragdoll.constraints.size(); boneIndex++)
	{
		ConstraintID bodyConstraint{ _ragdoll.constraints[boneIndex] };
		// 無効値であれば飛ばすでな。
		if (!bodyConstraint.IsValid())
		{
			continue;
		}

		ActiveRagdoll activeRagdoll;

		const EndPointFrame parentEndPoint{ PhysicsComponentAPI::GetEndPoint(bodyConstraint) };
		for (const EndPointFrame& childEndPoint : PhysicsComponentAPI::GetOtherEndPoints(bodyConstraint))
		{
			ConstraintID constraintID{ PhysicsComponentAPI::CreateInternalJointDriveConstraint(
				_entity,
				parentEndPoint.transformID,
				parentEndPoint.localPosition,
				parentEndPoint.localRotation) };

			PhysicsComponentAPI::AddEndPoint(
				constraintID,
				_entity,
				childEndPoint.localPosition,
				childEndPoint.localRotation);

			activeRagdoll.childBoneIndex.push_back(boneIndex);
			activeRagdoll.parentBoneIndex.push_back(parentIndices[boneIndex]);
			activeRagdoll.jointDriveConstraints.push_back(constraintID);
		}
	}
}

// ActiveRagdoll情報の破棄関数
void ActiveRagdollStorage::DestroyActiveRagdoll(RagdollID _ragdollID, const ActiveRagdoll& _activeRagdoll)
{
	AnimationComponentAPI::DestroyRagdoll(_ragdollID);
	for (auto jointDriveConstraint : _activeRagdoll.jointDriveConstraints)
	{
		PhysicsComponentAPI::DestroyConstraint(jointDriveConstraint);
	}
}