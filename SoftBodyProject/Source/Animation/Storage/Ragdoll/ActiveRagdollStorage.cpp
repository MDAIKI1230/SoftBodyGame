#include "PhysicsComponentAPI.h"
#include "AnimationComponentAPI.h"

#include "ActiveRagdollStorage.h"

// 作成関数
ActiveRagdollID ActiveRagdollStorage::Create(EntityID _entity, RagdollID _ragdollID, const Ragdoll& _ragdoll, const SkeletonData* _skeleton, const ActiveRagdollSetting& _setting, const BoneMask* _mask)
{
	ActiveRagdollID id{ CreateID(CountRagdollID()) };

	// 失敗した場合は無効値を返す
	if (!CreateActiveRagdoll(_entity, _ragdoll, _skeleton, _setting, _mask))
	{
		ReleaseID(id);
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
bool ActiveRagdollStorage::CreateActiveRagdoll(EntityID _entity, const Ragdoll& _ragdoll, const SkeletonData* _skeleton, const ActiveRagdollSetting& _setting, const BoneMask* _mask)
{
	if (_skeleton == nullptr)
	{
		return false;
	}

	ActiveRagdoll activeRagdoll;

	for (int boneIndex{ 0 }; boneIndex < _ragdoll.constraints.size(); boneIndex++)
	{
		ConstraintID bodyConstraint{ _ragdoll.constraints[boneIndex] };
		// 無効値であれば無効値を入れる
		if (!bodyConstraint.IsValid())
		{
			activeRagdoll.pointConstraints.emplace_back();
			activeRagdoll.pointTransformIDs.emplace_back();
			continue;
		}
		ConstraintTuning tuning;

		tuning.compliance = _setting.jointDriveCompliance;
		float weight{ _mask != nullptr ? _mask->weights[boneIndex] : 1.0f };
		tuning.maxForce = _setting.maxJointDriveForce * weight;

		const EndPointFrame parentEndPoint{ PhysicsComponentAPI::GetEndPoint(bodyConstraint) };
		for (const EndPointFrame& childEndPoint : PhysicsComponentAPI::GetOtherEndPoints(bodyConstraint))
		{
			// JointDrive拘束を作成してく
			ConstraintID jointDriveID{ PhysicsComponentAPI::CreateInternal<JointDriveConstraint>(
				_entity,
				parentEndPoint.transformID,
				parentEndPoint.localPosition,
				parentEndPoint.localRotation) };

			PhysicsComponentAPI::AddInternalEndPoint(
				jointDriveID,
				childEndPoint.transformID,
				childEndPoint.localPosition,
				childEndPoint.localRotation);

			PhysicsComponentAPI::SetTuning(jointDriveID, tuning);

			activeRagdoll.childBoneIndex.push_back(boneIndex);
			activeRagdoll.parentBoneIndex.push_back(_ragdoll.parentIndices[boneIndex]);
			activeRagdoll.jointDriveConstraints.push_back(jointDriveID);
		}

		const RagdollBodyLink& link{ _ragdoll.bodyLinks[boneIndex] };
		PhysicsTransformID boneTransformID{ _ragdoll.transforms[boneIndex] };
		Vector3 localPoint{ link.bodyFromBone * Vector3::ZERO };

		// point拘束を作成していく
		ConstraintID pointID{ PhysicsComponentAPI::CreateInternal<PointConstraint>(_entity, _ragdoll.transforms[boneIndex], localPoint) };

		// 内部の目標位置を拘束に渡すようTransformID
		PhysicsTransformID transformID{ PhysicsComponentAPI::CreateInternalPhysicsTransformID(
			_entity,
			Vector3::ZERO,
			Quaternion::IDENTITY,
			Vector3::ONE) };

		PhysicsComponentAPI::AddInternalEndPoint(
			pointID,
			transformID,
			Vector3::ZERO);

		PhysicsComponentAPI::SetTuning(pointID, tuning);

		activeRagdoll.pointConstraints.push_back(pointID);
		activeRagdoll.pointTransformIDs.push_back(transformID);
	}

	activeRagdoll.settings = _setting;

	activeRagdolls.push_back(std::move(activeRagdoll));

	return true;
}

// ActiveRagdoll情報の破棄関数
void ActiveRagdollStorage::DestroyActiveRagdoll(RagdollID _ragdollID, const ActiveRagdoll& _activeRagdoll)
{
	for (auto jointDriveConstraint : _activeRagdoll.jointDriveConstraints)
	{
		PhysicsComponentAPI::DestroyConstraint(jointDriveConstraint);
	}

	for (auto transformID : _activeRagdoll.pointTransformIDs)
	{
		PhysicsComponentAPI::DestroyPhysicsTransform(transformID);
	}
}