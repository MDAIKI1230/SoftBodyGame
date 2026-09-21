#include "FeatureIKSystem.h"

void FeatureIKSystem::Update(SkeletonInstanceStorage* _skeletonStorage, FeatureIKStorage* _featureIKStorage, SolverIKStorage* _solverIKStorage)
{
	HandIKUpdate(_skeletonStorage, _featureIKStorage, _solverIKStorage);
}

void FeatureIKSystem::HandIKUpdate(SkeletonInstanceStorage* _skeletonStorage, FeatureIKStorage* _featureIKStorage, SolverIKStorage* _solverIKStorage)
{
	for (FeatureIKID id : _featureIKStorage->GetHandIKIDRange())
	{
		SolverIKID solverIK{ _featureIKStorage->GetHandSolverIKID(id) };
		SkeletonID skeleton{ _featureIKStorage->GetHandSkeletonID(id) };
		const HandIK& handIK{ _featureIKStorage->GetHandIK(id) };
		TwoBoneIK& twoBoneIK{ _solverIKStorage->EditTwoBoneIK(_featureIKStorage->GetHandSolverIKID(id)) };
		
		const PoseBuffer& targetPose{ _skeletonStorage->GetTargetPose(skeleton) };

		twoBoneIK.rootPosition = targetPose.localPositions[twoBoneIK.rootBoneIndex];
		twoBoneIK.jointPosition = targetPose.localPositions[twoBoneIK.jointBoneIndex];
		twoBoneIK.endPosition = targetPose.localPositions[twoBoneIK.endBoneIndex];
	}
}