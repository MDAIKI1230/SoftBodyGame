#include "AssertMacros.h"

#include "PhysicsComponentAPI.h"

#include "RagdollStorage.h"

// 生成
RagdollID RagdollStorage::Create(EntityID _entity, SkeletonID _skeletonID, ModelHandle _model, const SkeletonInstanceData& _skeleton, const RagdollDefinition& _definition)
{
	RagdollID result{ CreateID(CountID()) };

	// ラグドール情報を生成時に失敗したら無効値を返す
	if (!CreateRagdoll(_entity, _skeletonID, _model, _skeleton, _definition))
	{
		return {};
	}

	neesInitializes.emplace_back(true);

	ids.push_back(result);

	ownerEntities.push_back(_entity);

	return result;
}

// 破棄
void RagdollStorage::Destroy(RagdollID _id)
{
	// 生存確認
	if (!IsAlive(_id))
	{
		return;
	}

	uint32_t denseIndex{ GetDenseIndex(_id) };
	uint32_t lastIndex{ static_cast<uint32_t>(ids.size() - 1) };

	for (auto transformID : ragdolls[denseIndex].transforms)
	{
		PhysicsComponentAPI::DestroyPhysicsTransform(transformID);
	}

	// Swap＆Pop
	if (denseIndex != lastIndex)
	{
		RagdollID movedID{ ids[lastIndex] };

		ragdolls[denseIndex] = std::move(ragdolls[lastIndex]);

		neesInitializes[denseIndex] = neesInitializes[lastIndex];

		ownerEntities[denseIndex] = ownerEntities[lastIndex];

		ids[denseIndex] = movedID;

		// 移動したInstanceの参照先を更新
		EditDenseIndex(movedID) = denseIndex;
	}

	ragdolls.pop_back();
	neesInitializes.pop_back();
	ownerEntities.pop_back();
	ids.pop_back();

	ReleaseID(_id);
}

// Ragdoll情報の作成関数
bool RagdollStorage::CreateRagdoll(EntityID _entity, SkeletonID _skeletonID, ModelHandle _model, const SkeletonInstanceData& _skeleton, const RagdollDefinition& _definition)
{
	std::unordered_map<std::string, uint32_t> nameToBoneIndex;

	Ragdoll ragdoll;

	ragdoll.skeleton = _skeletonID;

	const SkeletonData& skeletonData{ *_skeleton.skeletonData };

	for (uint32_t boneIndex{ 0 }; boneIndex < skeletonData.boneNames.size(); boneIndex++)
	{
		const std::string& boneName{ skeletonData.boneNames[boneIndex] };

		nameToBoneIndex[boneName] = boneIndex;
		// 作成用情報に対応するボーンの名前があるのなら、Bodyの作成にかかる。
		if (_definition.boneToBody.contains(boneName))
		{
			// ボディの情報
			const RagdollBodyDefinition& bodyDefinition{ _definition.boneToBody.at(boneName) };

			// トランスフォームを作ってからリジッドボディとコライダーを作成する
			PhysicsTransformID transformID{
				PhysicsComponentAPI::CreateInternalPhysicsTransformID(
					_entity,
					Vector3::ZERO,
					Quaternion::IDENTITY,
					Vector3::ONE) };

			ragdoll.transforms.push_back(transformID);

			ColliderID collider;

			// 種別に分岐する
			switch (bodyDefinition.colliderShape)
			{
			case ColliderType::SPHERE:
				collider = PhysicsComponentAPI::CreateInternalSphereCollider(
					_entity, transformID,
					bodyDefinition.colliderDefinition.sphere.radius
				);
				break;
			case ColliderType::CAPSULE:
				collider = PhysicsComponentAPI::CreateInternalCapsuleCollider(
					_entity, transformID,
					bodyDefinition.colliderDefinition.capsule.height,
					bodyDefinition.colliderDefinition.capsule.radius
				);
				break;
			case ColliderType::BOX:
				collider = PhysicsComponentAPI::CreateInternalBoxCollider(
					_entity, transformID,
					Vector3{
						bodyDefinition.colliderDefinition.box.width,
						bodyDefinition.colliderDefinition.box.height,
						bodyDefinition.colliderDefinition.box.depth
					}
				);
				break;
			default:
				// コライダー生成出来ないため失敗
				return false;
				break;
			}

			ragdoll.colliders.push_back(collider);

			// Body作りついでに、リンクを完成させる。

			RagdollBodyLink link;

			link.boneIndex = boneIndex;

			link.bodyID = PhysicsComponentAPI::CreateInternalRigidBody(_entity, transformID);

			link.bodyPositionInBoneSpace = bodyDefinition.bodyLocalPosition;
			link.bodyRotationInBoneSpace = bodyDefinition.bodyLocalRotation;

			link.bodyFromBone = MatGenerateFunc::InverseTRS(
				link.bodyPositionInBoneSpace,
				link.bodyRotationInBoneSpace,
				Vector3::ONE
			);

			// 質量の設定
			PhysicsComponentAPI::SetMass(link.bodyID, bodyDefinition.mass);

			ragdoll.bodyLinks.push_back(std::move(link));
		}
		else
		{
			RagdollBodyLink link;

			link.boneIndex = boneIndex;

			ragdoll.bodyLinks.push_back(std::move(link));

			// ボディない場合は無効値を入れておく

			ragdoll.colliders.emplace_back();
			ragdoll.transforms.emplace_back();

			continue;
		}
	}

	// Joint作成
	for (uint32_t boneIndex{ 0 }; boneIndex < skeletonData.boneNames.size(); boneIndex++)
	{
		const std::string& boneName{ skeletonData.boneNames[boneIndex] };

		if (_definition.boneToJoint.contains(boneName))
		{
			// Joint情報を取得
			const RagdollJointDefinition& jointDefinition{ _definition.boneToJoint.at(boneName) };

			const RagdollBodyLink& childLink{ ragdoll.bodyLinks[boneIndex] };

			const auto parentIt{ nameToBoneIndex.find(jointDefinition.parentBoneName) };

			if (parentIt == nameToBoneIndex.end())
			{
				MD_UNREACHABLE("Ragdoll joint parent bone was not found.");
			}

			const uint32_t parentIndex{ parentIt->second };

			const RagdollBodyLink& parentLink{ ragdoll.bodyLinks[parentIndex] };

			if (!childLink.bodyID.IsValid() || !parentLink.bodyID.IsValid())
			{
				MD_UNREACHABLE("Ragdoll joint requires valid parent and child bodies.");
			}

			// 子Bone空間から見たJoint
			const Matrix4x4 childBoneFromJoint{
				MatGenerateFunc::TRS(
					jointDefinition.constraintPositionLocalChild,
					jointDefinition.constraintRotationLocalChild,
					Vector3::ONE)
			};

			// 子Body空間から見たJoint
			const Matrix4x4 childBodyFromJoint{
				childLink.bodyFromBone *
				childBoneFromJoint
			};

			// Model空間から親Bone空間への変換
			const Matrix4x4 parentBoneFromModel{
				MatGenerateFunc::InverseTRS(
					skeletonData.bindModelPositions[parentIndex],
					skeletonData.bindModelRotations[parentIndex],
					skeletonData.bindModelScales[parentIndex])
			};

			// 親Body空間から見たJoint
			const Matrix4x4 parentBodyFromJoint{
				parentLink.bodyFromBone *
				parentBoneFromModel *
				skeletonData.bindModelMatrices[boneIndex] *
				childBoneFromJoint
			};

			// 位置/回転/スケールに分解
			Vector3 childJointPosition;
			Quaternion childJointRotation;
			Vector3 childJointScale;

			Transform::DecomposeTRS(
				childBodyFromJoint,
				childJointPosition,
				childJointRotation,
				childJointScale);

			Vector3 parentJointPosition;
			Quaternion parentJointRotation;
			Vector3 parentJointScale;

			Transform::DecomposeTRS(
				parentBodyFromJoint,
				parentJointPosition,
				parentJointRotation,
				parentJointScale);

			ConstraintID constraint;

			switch (jointDefinition.type)
			{
			case ConstraintType::ANGLE_LIMIT_HINGE:
				constraint =
					PhysicsComponentAPI::CreateInternalAngleLimitHingeConstraint(
						_entity,
						ragdoll.transforms[parentIndex],
						parentJointPosition,
						parentJointRotation);

				PhysicsComponentAPI::SetAngleMin(
					constraint,
					jointDefinition.angleLimit.hinge.minAngleRadians);

				PhysicsComponentAPI::SetAngleMax(
					constraint,
					jointDefinition.angleLimit.hinge.maxAngleRadians);
				break;

			case ConstraintType::LIMITED_BALL_JOINT:
				constraint =
					PhysicsComponentAPI::CreateInternalLimitedBallJointConstraint(
						_entity,
						ragdoll.transforms[parentIndex],
						parentJointPosition,
						parentJointRotation);

				PhysicsComponentAPI::SetSwingAngle(
					constraint,
					jointDefinition.angleLimit.swing.maxSwingAngleRadians);

				PhysicsComponentAPI::SetTwistAngleMin(
					constraint,
					jointDefinition.angleLimit.swing.minTwistAngleRadians);

				PhysicsComponentAPI::SetTwistAngleMax(
					constraint,
					jointDefinition.angleLimit.swing.maxTwistAngleRadians);
				break;

			default:
				MD_UNREACHABLE("Unsupported ragdoll joint type.");
			}

			PhysicsComponentAPI::AddInternalEndPoint(
				constraint,
				ragdoll.transforms[boneIndex],
				childJointPosition,
				childJointRotation);

			ragdoll.constraints.push_back(constraint);
		}
		else
		{
			// 拘束がない場合もあるので無効値を入れておく

			ragdoll.constraints.emplace_back();
		}
	}

	ragdolls.push_back(std::move(ragdoll));

	return true;
}