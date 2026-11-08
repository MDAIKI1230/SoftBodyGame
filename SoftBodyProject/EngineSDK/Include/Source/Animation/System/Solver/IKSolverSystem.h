#pragma once

#include "SolverIKStorage.h"
#include "PoseLayerStorage.h"

/*
	IK解消用システム
	解消用の構造体を順に解消していく。
*/
class IKSolverSystem
{
public:
	// IKの問題解消
	void Solve(PoseLayerStorage* _poseLayerStorage, SolverIKStorage* _solverIKStorage);
private:
	// 2関節IK用
	void TwoBoneSolve(PoseLayerStorage* _poseLayerStorage, SolverIKStorage* _solverIKStorage);
};