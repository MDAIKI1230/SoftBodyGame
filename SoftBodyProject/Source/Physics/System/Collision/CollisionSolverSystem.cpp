#include <algorithm>

#include "ServiceLocator.h"
#include "CollisionSolverSystem.h"

CollisionSolverSystem::CollisionSolverSystem() :
	K{ 0.4f },
	K_DELTA_TIME{ K * ServiceLocator::GetTimeManager()->GetFixedDeltaTime() },
	C{ 0.4f },
	ERP{ K_DELTA_TIME / (K_DELTA_TIME + C) },
	GAMMA{ 1 / (C + K_DELTA_TIME) }
{
	
}

void CollisionSolverSystem::FixedUpdate(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, ColliderStorage* _colliderStorage, CollisionManifoldBuffer* _manifoldBuffer)
{
	// 解消準備
	StartUp(_transformStorage, _bodyStorage, _colliderStorage, _manifoldBuffer);
	// 速度解消を指定回数分回す
	for (int i{ 0 }; i < VELOCITY_SOLVER_TIMES; i++)
	{
		VelocitySolver(_transformStorage, _bodyStorage, _manifoldBuffer);
	}
	// 修正された速度で位置を再計算
	ReCalcPosRot();
	// 位置/姿勢解消を指定回数分回す
	for (int i{ 0 }; i < POS_ROT_SOLVER_TIMES; i++)
	{
		PositionSolver(_transformStorage, _bodyStorage, _manifoldBuffer);
	}
	// 終了
	End(_transformStorage, _bodyStorage, _manifoldBuffer);
}

void CollisionSolverSystem::StartUp(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, ColliderStorage* _colliderStorage, CollisionManifoldBuffer* _manifoldBuffer)
{
	// メモリの確保
	contactConstraints.reserve(_manifoldBuffer->manifolds.size() * 2);
	solverBodies.reserve(_manifoldBuffer->manifolds.size());
	bodyMap.reserve(_manifoldBuffer->manifolds.size());

	// すべての衝突情報から拘束条件とソルバ用Bodyの作成をする
	for (auto& manifold : _manifoldBuffer->manifolds)
	{
		for (int i{ 0 }; i < manifold.pointCount; i++)
		{
			ContactConstraint contactConstraint;
			// SolverBodyのIndexを取得
			PhysicsTransformID transformID{ _colliderStorage->GetTransformID(manifold.colliderA) };
			contactConstraint.solverBodyAIndex = GetSolverBodyIndex(_transformStorage, _bodyStorage, transformID);
			transformID = _colliderStorage->GetTransformID(manifold.colliderB);
			contactConstraint.solverBodyBIndex = GetSolverBodyIndex(_transformStorage, _bodyStorage, transformID);

			contactConstraint.positionA = manifold.points[i].positionA;
			contactConstraint.positionB = manifold.points[i].positionB;
			contactConstraint.normal = manifold.normal;
			contactConstraint.penetration = manifold.points[i].penetration;

			contactConstraints.push_back(contactConstraint);
		}
	}
}

void CollisionSolverSystem::VelocitySolver(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, CollisionManifoldBuffer* _manifoldBuffer)
{
	for (auto& constraint : contactConstraints)
	{
		// 質量から両者がBodyを持っているかの判定をする(どちらかがBodyを持っているなら合計は0じゃないはず)
		float totalInvMass{ solverBodies[constraint.solverBodyAIndex].inverseMass + solverBodies[constraint.solverBodyBIndex].inverseMass };
		if (totalInvMass <= 0)
		{
			continue;
		}

		// biasを求める
		const float bias{ ERP / ServiceLocator::GetTimeManager()->GetFixedDeltaTime() * constraint.penetration };

		// 重心から衝突点ベクトル
		Vector3 rA{ constraint.positionA - solverBodies[constraint.solverBodyAIndex].position };
		Vector3 rB{ constraint.positionB - solverBodies[constraint.solverBodyBIndex].position };

		// 重心から衝突点ベクトルAと法線の外積
		Vector3 rACross{ Vector3::Cross(rA,constraint.normal) };
		// 重心から衝突点ベクトルBと法線の外積
		Vector3 rBCross{ Vector3::Cross(rB,constraint.normal) };

		// ヤコビアン
		Vector3 jacobian[4]{ constraint.normal,rACross,-constraint.normal,-rBCross };
		// 変化量ベクトル
		Vector3 deltaVector[4]{
			solverBodies[constraint.solverBodyAIndex].velocity,
			solverBodies[constraint.solverBodyAIndex].angularVelocity,
			solverBodies[constraint.solverBodyBIndex].velocity,
			solverBodies[constraint.solverBodyBIndex].angularVelocity,
		};
		// λを求める λ = -Jv - bias / M^-1
		// ヤコビアンと変化量ベクトルから速度拘束条件Jv = 0のJv作成
		float jv{ 0 };
		for (int i{ 0 }; i < 4; i++)
		{
			jv += Vector3::Dot(jacobian[i], deltaVector[i]);
		}

		// 質量と慣性テンソルが速度に影響する度合い
		float effectiveMass{
			solverBodies[constraint.solverBodyAIndex].inverseMass +
			Vector3::Dot(rACross,solverBodies[constraint.solverBodyAIndex].inverseInertiaTensor * rACross) +
			solverBodies[constraint.solverBodyBIndex].inverseMass +
			Vector3::Dot(rBCross,solverBodies[constraint.solverBodyBIndex].inverseInertiaTensor * rBCross)
		};

		// λ計算(CFMも適応)
		float lambda{ (jv + bias) / (effectiveMass + GAMMA) };

		float oldLambda{ constraint.accumulatedLambda };

		// 0未満にしない
		constraint.accumulatedLambda = std::max(oldLambda + lambda, 0.0f);

		float applyLambda{ constraint.accumulatedLambda - oldLambda };

		// A速度の解消
		solverBodies[constraint.solverBodyAIndex].velocity -= constraint.normal * applyLambda * solverBodies[constraint.solverBodyAIndex].inverseMass;
		solverBodies[constraint.solverBodyAIndex].angularVelocity -= solverBodies[constraint.solverBodyAIndex].inverseInertiaTensor * rACross * applyLambda;

		// B速度の解消
		solverBodies[constraint.solverBodyBIndex].velocity += constraint.normal * applyLambda * solverBodies[constraint.solverBodyBIndex].inverseMass;
		solverBodies[constraint.solverBodyBIndex].angularVelocity += solverBodies[constraint.solverBodyBIndex].inverseInertiaTensor * rBCross * applyLambda;

		// 摩擦
		FrictionSolver(solverBodies[constraint.solverBodyAIndex], rA, solverBodies[constraint.solverBodyBIndex], rB, constraint, effectiveMass);
	}
}

void CollisionSolverSystem::FrictionSolver(SolverBody& _bodyA, Vector3& _rA, SolverBody& _bodyB, Vector3& _rB, ContactConstraint& _constraint, float _effectiveMass)
{
	// 角速度まで含めた速度を計算
	Vector3 vA = _bodyA.velocity + Vector3::Cross(_bodyA.angularVelocity, _rA);
	Vector3 vB = _bodyB.velocity + Vector3::Cross(_bodyB.angularVelocity, _rB);

	// 相対速度計算
	Vector3 relativeVelocity = vA - vB;

	// 法線方向成分を取り除いて表面方向の速度を取り出す
	Vector3 tangentVelocity = relativeVelocity - (_constraint.normal * Vector3::Dot(relativeVelocity, _constraint.normal));

	// 表面方向の速度がほぼないなら何もしない
	if (tangentVelocity.Length() <= MathConstants::EPSILON)
	{
		return;
	}

	Vector3 tangent = tangentVelocity.Normalized();

	// 重心から衝突点ベクトルAと法線の外積
	Vector3 rACross{ Vector3::Cross(_rA,tangent) };
	// 重心から衝突点ベクトルBと法線の外積
	Vector3 rBCross{ Vector3::Cross(_rB,tangent) };

	float lambda = Vector3::Dot(relativeVelocity, tangent) / _effectiveMass;

	float maxFrictionLambda = _constraint.accumulatedLambda;

	float oldLambda = _constraint.accumulatedFrictionLambda;
	_constraint.accumulatedFrictionLambda =
		std::clamp(oldLambda + lambda, 0.0f, maxFrictionLambda);

	float applyLambda = _constraint.accumulatedFrictionLambda - oldLambda;

	// A速度の摩擦
	solverBodies[_constraint.solverBodyAIndex].velocity -= tangent * applyLambda * solverBodies[_constraint.solverBodyAIndex].inverseMass;
	solverBodies[_constraint.solverBodyAIndex].angularVelocity -= solverBodies[_constraint.solverBodyAIndex].inverseInertiaTensor * rACross * applyLambda;

	// B速度の摩擦
	solverBodies[_constraint.solverBodyBIndex].velocity += tangent * applyLambda * solverBodies[_constraint.solverBodyBIndex].inverseMass;
	solverBodies[_constraint.solverBodyBIndex].angularVelocity += solverBodies[_constraint.solverBodyBIndex].inverseInertiaTensor * rBCross * applyLambda;
}


void CollisionSolverSystem::ReCalcPosRot()
{
	for (auto& body : solverBodies)
	{
		if (body.inverseMass != 0.0f)
		{
			// 変化した速度から位置を再計算
			// 位置 + 修正後のベクトル
			body.position = body.pastPos + body.velocity * ServiceLocator::GetTimeManager()->GetFixedDeltaTime();

			// Δω
			Vector3 deltaAngularVelocity{ body.angularVelocity * ServiceLocator::GetTimeManager()->GetFixedDeltaTime() };
			// Δωの四元数を作る
			Quaternion rotOmega{ Quaternion::AngleAxis(deltaAngularVelocity.Length(),deltaAngularVelocity) };
			// 回転＋修正後の角速度の回転
			body.rotation = body.pastRot * rotOmega;
		}
	}

	for (auto& constraint : contactConstraints)
	{
		constraint.accumulatedLambda = 0.0f;
	}
}

void CollisionSolverSystem::PositionSolver(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, CollisionManifoldBuffer* _manifoldBuffer)
{
	for (auto& constraint : contactConstraints)
	{
		// 質量から両者がBodyを持っているかの判定をする(どちらかがBodyを持っているなら合計は0じゃないはず)
		float totalInvMass{ solverBodies[constraint.solverBodyAIndex].inverseMass + solverBodies[constraint.solverBodyBIndex].inverseMass };
		if (totalInvMass <= 0)
		{
			continue;
		}

		// biasを求める
		float bias{ ERP / ServiceLocator::GetTimeManager()->GetFixedDeltaTime() * constraint.penetration };

		// 重心から衝突点ベクトル
		Vector3 rA{ constraint.positionA - solverBodies[constraint.solverBodyAIndex].position };
		Vector3 rB{ constraint.positionB - solverBodies[constraint.solverBodyBIndex].position };

		// 重心から衝突点ベクトルAと法線の外積
		Vector3 rACross{ Vector3::Cross(rA,constraint.normal) };
		// 重心から衝突点ベクトルBと法線の外積
		Vector3 rBCross{ Vector3::Cross(rB,constraint.normal) };

		// 質量と慣性テンソルが速度に影響する度合い
		float effectiveMass{
			solverBodies[constraint.solverBodyAIndex].inverseMass +
			Vector3::Dot(rACross,solverBodies[constraint.solverBodyAIndex].inverseInertiaTensor * rACross) +
			solverBodies[constraint.solverBodyBIndex].inverseMass +
			Vector3::Dot(rBCross,solverBodies[constraint.solverBodyBIndex].inverseInertiaTensor * rBCross)
		};
		// 重なり深さを決める(0が最小になるように)
		float depth = std::max(constraint.penetration - POSITION_SOLVE_SLOP, 0.0f);
		// 解消の割合から解消量を計算
		float correction = POSITION_SOLVE_PERCENT * depth;

		// λ計算(CFMも適応)
		float lambda{ correction / effectiveMass };

		float oldLambda{ constraint.accumulatedLambda };

		// 0未満にしない
		constraint.accumulatedLambda = std::max(oldLambda + lambda, 0.0f);

		float applyLambda{ constraint.accumulatedLambda - oldLambda };

		// 解消した分だけ減らす(0が最小になるように)
		constraint.penetration = std::max(constraint.penetration - correction, 0.0f);

		// Aの位置/姿勢制御
		solverBodies[constraint.solverBodyAIndex].position -= constraint.normal * applyLambda * solverBodies[constraint.solverBodyAIndex].inverseMass;
		Vector3 angVec{ solverBodies[constraint.solverBodyAIndex].inverseInertiaTensor * rACross * applyLambda };
		Quaternion rotOmega{ Quaternion::AngleAxis(angVec.Length(), -angVec) };
		solverBodies[constraint.solverBodyAIndex].rotation *= rotOmega;

		// Bの位置/姿勢制御
		solverBodies[constraint.solverBodyBIndex].position += constraint.normal * applyLambda * solverBodies[constraint.solverBodyBIndex].inverseMass;
		angVec = solverBodies[constraint.solverBodyBIndex].inverseInertiaTensor * rBCross * applyLambda;
		rotOmega = Quaternion::AngleAxis(angVec.Length(), angVec);
		solverBodies[constraint.solverBodyBIndex].rotation *= rotOmega;
	}
}

void CollisionSolverSystem::End(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, CollisionManifoldBuffer* _manifoldBuffer)
{
	// 結果を反映していく
	for (auto& result : solverBodies)
	{
		// 質量が0ならBodyはないので書かない
		if (result.inverseMass > 0)
		{
			uint32_t bodyIndex{ _bodyStorage->GetDenseIndex(result.bodyID) };
			uint32_t transformIndex{ _transformStorage->GetDenseIndex(result.transformID) };
			_transformStorage->position[transformIndex] = result.position;
			_bodyStorage->velocity[bodyIndex] = result.velocity;
			_transformStorage->rotation[transformIndex] = result.rotation;
			_bodyStorage->angularVelocity[bodyIndex] = result.angularVelocity;
		}
	}
	// リセット
	contactConstraints.clear();
	solverBodies.clear();
	bodyMap.clear();

	_manifoldBuffer->Clear();
}

uint32_t CollisionSolverSystem::CreateSolverBody(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, PhysicsTransformID& _transformID, BodyID& _bodyID)
{
	SolverBody body;

	// 情報をひとつづつ埋めていく
	body.bodyID = _bodyID;
	body.transformID = _transformID;
	uint32_t bodyIndex{ _bodyStorage->GetDenseIndex(_bodyID) };
	uint32_t transformIndex{ _transformStorage->GetDenseIndex(_transformID) };
	body.pastPos = _bodyStorage->pastPos[bodyIndex];
	body.position = _transformStorage->position[transformIndex];
	body.velocity = _bodyStorage->velocity[bodyIndex];
	body.pastRot = _bodyStorage->pastRot[bodyIndex];
	body.rotation = _transformStorage->rotation[transformIndex];
	body.angularVelocity = _bodyStorage->angularVelocity[bodyIndex];
	body.inverseMass = _bodyStorage->inverseMass[bodyIndex];
	body.inverseInertiaTensor = _bodyStorage->worldInverseInertiaTensor[bodyIndex];

	// インデックス取ってから追加
	uint32_t result{ static_cast<uint32_t>(solverBodies.size()) };
	solverBodies.push_back(body);
	return result;
}

uint32_t CollisionSolverSystem::CreateSolverBody(PhysicsTransformStorage* _transformStorage, PhysicsTransformID& _transformID)
{
	SolverBody body;

	// 情報をひとつづつ埋めていく(Bodyが存在しない版)
	body.transformID = _transformID;
	uint32_t transformIndex{ _transformStorage->GetDenseIndex(_transformID) };
	body.pastPos = _transformStorage->position[transformIndex];
	body.position = _transformStorage->position[transformIndex];
	body.velocity = Vector3::ZERO;
	body.pastRot = _transformStorage->rotation[transformIndex];
	body.rotation = _transformStorage->rotation[transformIndex];
	body.angularVelocity = Vector3::ZERO;;
	body.inverseMass = 0;
	body.inverseInertiaTensor = Matrix4x4::Zero();

	// インデックス取ってから追加
	uint32_t result{ static_cast<uint32_t>(solverBodies.size()) };
	solverBodies.push_back(body);
	return result;
}

uint32_t   CollisionSolverSystem::GetSolverBodyIndex(
	PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, PhysicsTransformID& _transformID)
{
	uint32_t result;
	
	BodyID bodyID;
	// Bodyがあるかの確認
	if (_bodyStorage->TryGet(_transformID, bodyID))
	{
		// MAPを確認してあったらそれを使う
		if (bodyMap.contains(_transformID))
		{
			result = bodyMap[_transformID];
		}
		else
		{
			// SolverBodyがないので作る
			if (_bodyStorage->IsAlive(bodyID))
			{
				// Bodyある版の作成
				result = CreateSolverBody(_transformStorage, _bodyStorage, _transformID, bodyID);
				bodyMap[_transformID] = result;
			}
			else
			{
				// Bodyない版の作成
				result = CreateSolverBody(_transformStorage, _transformID);
				bodyMap[_transformID] = result;
			}
		}
	}
	else
	{
		// Bodyない版の作成
		result = CreateSolverBody(_transformStorage, _transformID);
		bodyMap[_transformID] = result;
	}

	return result;
}
