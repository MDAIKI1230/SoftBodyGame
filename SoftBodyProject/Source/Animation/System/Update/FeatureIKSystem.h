#pragma once

#include "SkeletonInstanceStorage.h"
#include "FeatureIKStorage.h"
#include "SolverIKStorage.h"

/*
	HandIkなどのIKのパラメータから解く用のIKのパラメータを決めていく。
*/
class  FeatureIKSystem
{
public:
	void Update(SkeletonInstanceStorage* _skeletonStorage, FeatureIKStorage* _featureIKStorage, SolverIKStorage* _solverIKStorage);
private:
	void HandIKUpdate(SkeletonInstanceStorage* _skeletonStorage, FeatureIKStorage* _featureIKStorage, SolverIKStorage* _solverIKStorage);
};