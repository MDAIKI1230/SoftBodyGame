#pragma once

#include "PhysicsTransformStorage.h"
#include "BodyStorage.h"
#include "SolverBodyBuffer.h"

class SolverBodyBuildSystem
{
public:
	void Build(PhysicsTransformStorage* _transformStorage, BodyStorage* _bodyStorage, SolverBodyBuffer* _solverBodyBuffer);
private:
	/// <summary>
	/// 情報からSolverBodyを作る
	/// </summary>
	/// <returns>インデックス</returns>
	uint32_t CreateSolverBody(PhysicsTransformStorage* _transformStorage, BodyStorage* _bodyStorage, PhysicsTransformID _transformID, BodyID _bodyID, SolverBodyBuffer* _solverBodyBuffer);
	/// <summary>
	/// 情報からSolverBodyを作る(Bodyがない版)
	/// </summary>
	/// <returns>インデックス</returns>
	uint32_t CreateSolverBody(PhysicsTransformStorage* _transformStorage, PhysicsTransformID _transformID, SolverBodyBuffer* _solverBodyBuffer);
};
