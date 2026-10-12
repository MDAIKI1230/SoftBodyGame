#include "PhysicsComponentAPI.h"
#include "AnimationComponentAPI.h"

#include "ActiveRagdollStorage.h"

// 作成関数
ActiveRagdollID ActiveRagdollStorage::Create(EntityID _entity, RagdollID _ragdollID, const Ragdoll& _ragdoll, const SkeletonData* _skeleton, const ActiveRagdollDefinition& _definition)
{
	ActiveRagdollID id{ CreateID(CountRagdollID()) };

	// 失敗した場合は無効値を返す
	if (!CreateActiveRagdoll(_entity, _ragdoll, _skeleton, _definition))
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
bool ActiveRagdollStorage::CreateActiveRagdoll(EntityID _entity, const Ragdoll& _ragdoll, const SkeletonData* _skeleton, const ActiveRagdollDefinition& _definition)
{
	if (_skeleton == nullptr)
	{
		return false;
	}

	const ActiveRagdollSetting& setting{ _definition.settings };

	ActiveRagdoll activeRagdoll;
	activeRagdoll.rotationWeights.ReSize(static_cast<uint32_t>(_skeleton->Size()), 1.0f);
	activeRagdoll.positionWeights.ReSize(static_cast<uint32_t>(_skeleton->Size()), 1.0f);

	if (_definition.isRotationWeight)
	{
		activeRagdoll.rotationWeights = _definition.rotationWeights;
	}
	if (_definition.isPositionWeight)
	{
		activeRagdoll.positionWeights = _definition.positionWeights;
	}

	// 姿勢制御用拘束生成
	for (int boneIndex{ 0 }; boneIndex < _ragdoll.constraints.size(); boneIndex++)
	{
		ConstraintID bodyConstraint{ _ragdoll.constraints[boneIndex] };
		// 無効値であれば無効値を入れる
		if (!bodyConstraint.IsValid())
		{
			activeRagdoll.positionConstraints.emplace_back();
			activeRagdoll.pointTransformIDs.emplace_back();
			continue;
		}
		ConstraintTuning rotationTuning;

		rotationTuning.compliance = setting.rotationConstraintCompliances[boneIndex];
		rotationTuning.maxForce = setting.roatationConstraintMaxPowers[boneIndex] *
			activeRagdoll.rotationWeights.weights[boneIndex];

		const EndPointFrame& parentEndPoint{ PhysicsComponentAPI::GetEndPoint(bodyConstraint) };
		for (const EndPointFrame& childEndPoint : PhysicsComponentAPI::GetOtherEndPoints(bodyConstraint))
		{
			// JointDrive拘束を作成してく
			ConstraintID jointDriveID{ PhysicsComponentAPI::CreateInternal<JointDriveConstraint>(
				_entity,
				parentEndPoint.transformID,
				parentEndPoint.localPosition,
				parentEndPoint.localRotation) };

			PhysicsComponentAPI::AddInternalEndPoint<JointDriveConstraint>(
				jointDriveID,
				childEndPoint.transformID,
				childEndPoint.localPosition,
				childEndPoint.localRotation);

			PhysicsComponentAPI::SetTuning(jointDriveID, rotationTuning);

			activeRagdoll.childBoneIndex.push_back(boneIndex);
			activeRagdoll.parentBoneIndex.push_back(_ragdoll.parentIndices[boneIndex]);
			activeRagdoll.rotationConstraints.push_back(jointDriveID);
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

		PhysicsComponentAPI::AddInternalEndPoint<PointConstraint>(
			pointID,
			transformID,
			Vector3::ZERO);

		ConstraintTuning positionTuning;

		positionTuning.compliance = setting.positionConstraintCompliances[boneIndex];
		positionTuning.maxForce = setting.positionConstraintMaxPowers[boneIndex] *
			activeRagdoll.positionWeights.weights[boneIndex];

		PhysicsComponentAPI::SetTuning(pointID, positionTuning);

		activeRagdoll.positionConstraints.push_back(pointID);
		activeRagdoll.pointTransformIDs.push_back(transformID);
	}

	// 腰用拘束生成
	uint32_t pelvisBoneIndex{ _ragdoll.roles[static_cast<uint32_t>(RagdollBoneRole::PELVIS)] };
	BodyID pelvisBodyID{ _ragdoll.bodyLinks[pelvisBoneIndex].bodyID };
	if (!pelvisBodyID.IsValid())
	{
		return false;
	}

	activeRagdoll.pelvisConstraintTransformID = PhysicsComponentAPI::CreateInternalPhysicsTransformID(
		_entity,
		Vector3::ZERO,
		Quaternion::IDENTITY,
		Vector3::ONE);

	activeRagdoll.pelvisConstraintID = PhysicsComponentAPI::CreateInternal<PointOnLineConstraint>(_entity, activeRagdoll.pelvisConstraintTransformID, Vector3::ZERO);

	PhysicsComponentAPI::AddEndPoint<PointOnLineConstraint>(activeRagdoll.pelvisConstraintID, pelvisBodyID, Vector3::ZERO);

	activeRagdoll.settings = setting;

	activeRagdolls.push_back(std::move(activeRagdoll));

	return true;
}

// ActiveRagdoll情報の破棄関数
void ActiveRagdollStorage::DestroyActiveRagdoll(RagdollID _ragdollID, const ActiveRagdoll& _activeRagdoll)
{
	for (auto jointDriveConstraint : _activeRagdoll.rotationConstraints)
	{
		PhysicsComponentAPI::DestroyConstraint(jointDriveConstraint);
	}

	for (auto transformID : _activeRagdoll.pointTransformIDs)
	{
		PhysicsComponentAPI::DestroyPhysicsTransform(transformID);
	}

	PhysicsComponentAPI::DestroyConstraint(_activeRagdoll.pelvisConstraintID);
	PhysicsComponentAPI::DestroyPhysicsTransform(_activeRagdoll.pelvisConstraintTransformID);
}