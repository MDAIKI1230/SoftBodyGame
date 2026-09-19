#include "NamericalAnalysis.h"

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

		const Ragdoll& ragdoll{ _ragdollStorage->EditRagdoll(id) };

		// Animation Pose → Physics Body
		SkeletonID skeletonID{ ragdoll.skeleton };

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
		for (int boneIndex{ 0 }; boneIndex < ragdoll.bodyLinks.size(); boneIndex++)
		{
			const RagdollBodyLink& link{ ragdoll.bodyLinks[boneIndex] };

			if (!link.bodyID.IsValid())
			{
				continue;
			}

			Matrix4x4 boneFromBody{ MatGenerateFunc::TRS(
				link.bodyPositionInBoneSpace,
				link.bodyRotationInBoneSpace,
				Vector3::ONE) };

			Matrix4x4 worldFromBody{
				_skeletonStorage->GetWorldFromModel(skeletonID) *
				modelMatrices[boneIndex] *
				boneFromBody };

			Vector3 position;
			Quaternion rotation;
			Vector3 scale;
			Transform::DecomposeTRS(worldFromBody, position, rotation, scale);

			PhysicsComponentAPI::SetRigidBodyPosition(link.bodyID, position);
			PhysicsComponentAPI::SetRigidBodyRoatation(link.bodyID, rotation);
			PhysicsComponentAPI::SetRigidBodyScale(link.bodyID, scale);
		}

		_ragdollStorage->SetNeedInitialize(id, false);
	}
}

// Physics実行後
void RagdollSystem::PostPhysicsFixedUpdate(SkeletonInstanceStorage* _skeletonStorage, RagdollStorage* _ragdollStorage)
{
	/*
	最終的には、姿勢のアウトプットの位置/回転/スケールを更新したい

	すべてのBoneで親->子の順番で処理をする

	手順：
		リジッドボディの姿勢を得る
							|
		　Bodyオフセットの逆変換
						   ↓
		Boneのワールド姿勢にゲット
							|
	　Modelのワールド変換の逆変換
						   ↓
		Boneのモデルから見たローカル姿勢ゲット
							|
		　親ボーンの変換を逆変換
						   ↓
		Boneの親から見たローカル姿勢ゲット

	リンクがない場合はTargetPoseのLocalMatrixを使う。

		TRSへ分解などの数値更新(outputに入れる)
*/
	for (auto& ragdoll : _ragdollStorage->EditRagdollRange())
	{
		// ラグドールと対応したスケルトンID
		SkeletonID skeletonID{ ragdoll.skeleton };

		// スケルトンデータ
		SkeletonInstanceData& skeleton{ _skeletonStorage->EditSkeletonInstanceData(skeletonID) };

		// モデルのワールド変換を逆変換する用行列
		Matrix4x4 modelFromWorld{ NamericalAnalysis::GaussJordan(_skeletonStorage->GetWorldFromModel(skeletonID)) };
		for (int boneIndex{ 0 }; boneIndex < ragdoll.bodyLinks.size(); boneIndex++)
		{
			// リンク
			const RagdollBodyLink& link{ ragdoll.bodyLinks[boneIndex] };
			// 親ボーン
			uint32_t parentIndex{ skeleton.skeletonData->parentIndices[boneIndex] };
			// 最終的な結果になる行列(親ボーンからのローカル行列)
			Matrix4x4 localMatrix;
			// BodyID
			BodyID bodyID{ link.bodyID };
			
			// リンクがあるボーン
			if (bodyID.IsValid())
			{
				// リジッドボディの姿勢を得る
				Vector3 position{ PhysicsComponentAPI::GetRigidBodyPosition(bodyID) };
				Quaternion rotation{ PhysicsComponentAPI::GetRigidBodyRoatation(bodyID) };

				Matrix4x4 worldBodyMatrix{ MatGenerateFunc::TRS(position,rotation,Vector3::ONE) };

				// リジッドボディのオフセットを逆変換してボーンのワールド姿勢を得る
				Matrix4x4 worldBoneMatrix{ worldBodyMatrix * link.bodyFromBone  };

				// ボーンのワールド姿勢をモデルからのローカル姿勢に変換
				Matrix4x4 modelMatrix{ modelFromWorld * worldBoneMatrix };

				// 上の行列を親ボーンからのローカル姿勢に変換

				// parentがルートの場合も加味する
				localMatrix =
					parentIndex == UINT32_MAX
					? modelMatrix
					: skeleton.outputPose.boneFromModelMatrices[parentIndex] * modelMatrix;
			}
			// BodyIDが無効値なのでリンクされてないBone
			else
			{
				localMatrix = skeleton.outputPose.localMatrices[boneIndex];
			}

			// TRS情報をアウトプットに入れる
			Vector3 localPosition;
			Quaternion localRotation;
			Vector3 decomposedScale;

			Transform::DecomposeTRS(
				localMatrix,
				localPosition,
				localRotation,
				decomposedScale
			);

			// RigidBodyを持つBoneのScaleはアニメーション側から維持する
			const Vector3 localScale{
				bodyID.IsValid()
					? skeleton.outputPose.localScales[boneIndex]
					: decomposedScale
			};

			// 維持したScaleで行列を作り直す
			localMatrix = MatGenerateFunc::TRS(
				localPosition,
				localRotation,
				localScale
			);

			skeleton.outputPose.localMatrices[boneIndex] = localMatrix;
			skeleton.outputPose.localPositions[boneIndex] = localPosition;
			skeleton.outputPose.localRotations[boneIndex] = localRotation;
			skeleton.outputPose.localScales[boneIndex] = localScale;

			// 子の計算の準備をする

			// 逆行列も計算
			Matrix4x4 boneFromParent{ MatGenerateFunc::InverseTRS(
				skeleton.outputPose.localPositions[boneIndex],
				skeleton.outputPose.localRotations[boneIndex],
				skeleton.outputPose.localScales[boneIndex]
			) };

			if (parentIndex == UINT32_MAX)
			{
				// モデルからのボーンのローカル姿勢
				skeleton.outputPose.modelFromBoneMatrices[boneIndex] = localMatrix;
				// 上の逆行列
				skeleton.outputPose.boneFromModelMatrices[boneIndex] = boneFromParent;
			}
			else
			{
				// モデルからのボーンのローカル姿勢
				skeleton.outputPose.modelFromBoneMatrices[boneIndex] =
					skeleton.outputPose.modelFromBoneMatrices[parentIndex] * localMatrix;

				// 上の逆行列
				skeleton.outputPose.boneFromModelMatrices[boneIndex] =
					boneFromParent * skeleton.outputPose.boneFromModelMatrices[parentIndex];
			}
		}
	}
}