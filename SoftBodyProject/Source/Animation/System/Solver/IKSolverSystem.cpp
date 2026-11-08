#include <algorithm>

#include "IKSolverSystem.h"

// IKの問題解消
void IKSolverSystem::Solve(PoseLayerStorage* _poseLayerStorage, SolverIKStorage* _solverIKStorage)
{
	TwoBoneSolve(_poseLayerStorage, _solverIKStorage);
}

// 2関節IK用
void IKSolverSystem::TwoBoneSolve(PoseLayerStorage* _poseLayerStorage, SolverIKStorage* _solverIKStorage)
{
	for (SolverIKID id : _solverIKStorage->GetTwoBoneIKIDRange())
	{
		// ポーズレイヤー
		PoseLayer& poseLayer{ _poseLayerStorage->EditPoseLayer(_solverIKStorage->GetTwoBoneIKPoseLayerID(id)) };
		// IK構造体
		const TwoBoneIK& ik{ _solverIKStorage->GetTwoBoneIK(id) };

		// ルート->目標点をＸ軸
		Vector3 baseX{ ik.targetPosition - ik.rootPosition };
		float distance{ baseX.Length() };
		baseX.Normalize();
		// 上のベクトルと直角に交わる方向をPoleを使い決める
		Vector3 baseY{ ik.polePosition - ik.rootPosition };
		baseY -= baseX * Vector3::Dot(baseX, baseY);
		baseY.Normalize();
		/*

			3次元をルート->目標点と直角なベクトルもう一つの平面上での交点を求める
								空間の基底ベクトル(B)
							   ルート(R) = (0, 0)
							   目標点(T) = (d, 0)
					   途中ジョイント(J) = (x, y)

					  ルートジョイントと途中ジョイントの円
							   x^2 + y^2 = (L_1)^2
						   途中ジョイントと目標点の円
						 (x - d)^2 + y^2 = (L_2)^2

						この二つの式を満たすx,yを求める
			x^2 +y^2 - ((x - d)^2 + y^2) = (L_1)^2 - (L_2)^2
				 x^2 - (x^2 - 2dx + d^2) = (L_1)^2 - (L_2)^2
							   2dx - d^2 = (L_1)^2 - (L_2)^2
									   x = ((L_1)^2 - (L_2)^2) / 2d

						　　　 x^2 + y^2 = (L_1)^2に代入して
									   y = ±√((L_1)^2 - x^2)

					   この、x,yを使って中間ジョイントの位置を計算
									   J = R + xB.x + yB.y

		*/

		float L1Sqr{ (ik.jointPosition - ik.rootPosition).LengthSqr() };
		float L2Sqr{ (ik.targetPosition - ik.jointPosition).LengthSqr() };

		float x{ (L1Sqr - L2Sqr) / (2 * distance) };
		float y{ std::sqrtf(L1Sqr - (x * x)) };

		// ワールドに戻す

		Vector3 j{ ik.rootPosition + baseX * x + baseY * y };

		// 相対回転を求めてターゲットポーズに代入
		Vector3 rootToJ{ j - ik.rootPosition };
		Vector3 rootToJoint{ ik.jointPosition - ik.rootPosition };
		Quaternion rootRot{ Quaternion::FromToRotation(rootToJoint,rootToJ) };

		Vector3 jToTarget{ ik.targetPosition - j };
		Vector3 jointToTarget{ ik.targetPosition - ik.jointPosition };
		Quaternion jointRot{ Quaternion::FromToRotation(jointToTarget,jToTarget) };

		// ウェイトも適応するよ。
		float weight{ std::clamp(ik.positionWeight, 0.0f, 1.0f) };

		poseLayer.pose.localRotations[ik.rootBoneIndex] = Quaternion::Slerp(
			poseLayer.pose.localRotations[ik.rootBoneIndex],
			rootRot,
			weight);

		poseLayer.pose.localRotations[ik.jointBoneIndex] = Quaternion::Slerp(
			poseLayer.pose.localRotations[ik.jointBoneIndex],
			jointRot,
			weight);
	}
}