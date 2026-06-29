#pragma once

#include <vector>
#include <unordered_map>

#include "CollisionManifoldBuffer.h"

#include "PhysicsTransformStorage.h"
#include "RigidBodyStorage.h"

#include "ContactConstraint.h"
#include "SolverBody.h"

class CollisionSolverSystem
{
public:
	// 更新
	void FixedUpdate(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, CollisionManifoldBuffer* _manifoldBuffer);
private:
	// 準備
	void StartUp(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, CollisionManifoldBuffer* _manifoldBuffer);
	// 位置解決
	void PositionSolver(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, CollisionManifoldBuffer* _manifoldBuffer);
	// 速度解決
	void VelocitySolver(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, CollisionManifoldBuffer* _manifoldBuffer);
	// 姿勢解決
	void OrientationSolver(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, CollisionManifoldBuffer* _manifoldBuffer);
	// 回転速度解決
	void RotationSolver(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, CollisionManifoldBuffer* _manifoldBuffer);
	// 終わり
	void End(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage);
	/// <summary>
	/// 情報からSolverBodyを作る
	/// </summary>
	/// <returns>インデックス</returns>
	uint32_t CreateSolverBody(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, PhysicsTransformID& _transformID, BodyID& _bodyID);
	/// <summary>
	/// 情報からSolverBodyを作る(Bodyがない版)
	/// </summary>
	/// <returns>インデックス</returns>
	uint32_t CreateSolverBody(PhysicsTransformStorage* _transformStorage, PhysicsTransformID& _transformID);
	// BodyIndexを返してくれる関数
	uint32_t  GetSolverBodyIndex(
		PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, BodyID& _bodyID);
private:
	std::vector<ContactConstraint> contactConstraints;
	std::vector<SolverBody> solverBodies;
	std::unordered_map<PhysicsTransformID, uint32_t> bodyMap;
};
