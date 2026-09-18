#include "PoseMixer.h"

void PoseMixer::MakeTargetPose(PoseBuffer& _targetPose, const PoseLayer& _layer)
{
	for (int boneIndex{ 0 }; boneIndex < _targetPose.localMatrices.size(); boneIndex++)
	{
		// 重み
		float weight{ _layer.mask.weights[boneIndex] * _layer.weight };
		// ポーズ
		const PoseBuffer& pose{ _layer.pose };

		_targetPose.localPositions[boneIndex] = Vector3::Lerp(
			_targetPose.localPositions[boneIndex],
			pose.localPositions[boneIndex],
			weight
		);
		_targetPose.localRotations[boneIndex] = Quaternion::Slerp(
			_targetPose.localRotations[boneIndex],
			pose.localRotations[boneIndex],
			weight
		);
		_targetPose.localScales[boneIndex] = Vector3::Lerp(
			_targetPose.localScales[boneIndex],
			pose.localScales[boneIndex],
			weight
		);

		_targetPose.localRotations[boneIndex].Normalize();

		_targetPose.localMatrices[boneIndex] = MatGenerateFunc::TRS(
				_targetPose.localPositions[boneIndex],
				_targetPose.localRotations[boneIndex],
				_targetPose.localScales[boneIndex]
			);
	}
}