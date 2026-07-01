#include "ServiceLocator.h"
#include "CollisionSolverSystem.h"

void CollisionSolverSystem::FixedUpdate(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, ColliderStorage* _colliderStorage, CollisionManifoldBuffer* _manifoldBuffer)
{
	StartUp(_transformStorage, _bodyStorage, _colliderStorage, _manifoldBuffer);
	VelocitySolver(_transformStorage, _bodyStorage, _manifoldBuffer);
	ReCalcPosRot();
	PositionSolver(_transformStorage, _bodyStorage, _manifoldBuffer);
	End(_transformStorage, _bodyStorage);
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

			contactConstraint.position = manifold.points[i].position;
			contactConstraint.normal =  manifold.normal;
			contactConstraint.penetration = manifold.points[i].penetration;

			contactConstraints.push_back(contactConstraint);
		}
	}
}

void CollisionSolverSystem::VelocitySolver(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, CollisionManifoldBuffer* _manifoldBuffer)
{
	for (auto& contactConstraint : contactConstraints)
	{
		float totalInvMass{ solverBodies[contactConstraint.solverBodyAIndex].inverseMass + solverBodies[contactConstraint.solverBodyBIndex].inverseMass };
		if (totalInvMass == 0)
		{
			continue;
		}

		// 重心から衝突点ベクトル
		Vector3 rA{ contactConstraint.position - solverBodies[contactConstraint.solverBodyAIndex].position };
		Vector3 rB{ contactConstraint.position - solverBodies[contactConstraint.solverBodyBIndex].position };

		// 相対速度
		Vector3 vA{ solverBodies[contactConstraint.solverBodyAIndex].velocity +
			Vector3::Cross(solverBodies[contactConstraint.solverBodyAIndex].angularVelocity, rA) };

		Vector3 vB{ solverBodies[contactConstraint.solverBodyBIndex].velocity +
			Vector3::Cross(solverBodies[contactConstraint.solverBodyBIndex].angularVelocity, rB) };

		Vector3 relativeVec{ vA - vB };

		float vn{ Vector3::Dot(relativeVec,contactConstraint.normal) };
		if (vn > 0.0f)
		{
			continue;
		}
		// 質量係数( mass01 * mass02 / mass01 + mass02)
		float massCoefficient{ solverBodies[contactConstraint.solverBodyAIndex].inverseMass + solverBodies[contactConstraint.solverBodyBIndex].inverseMass };

		// 回転項
		Vector3 rotationA{ Vector3::Cross((solverBodies[contactConstraint.solverBodyAIndex].inverseInertiaTensor * Vector3::Cross(rA,contactConstraint.normal)),rA) };
		Vector3 rotationB{ Vector3::Cross((solverBodies[contactConstraint.solverBodyBIndex].inverseInertiaTensor * Vector3::Cross(rB,contactConstraint.normal)),rB) };
		float rotationCoefficient{ Vector3::Dot(rotationA + rotationB,contactConstraint.normal) };

		// 撃力計算
		float j{ -vn / massCoefficient };

		Vector3 impulse = contactConstraint.normal * j;

		solverBodies[contactConstraint.solverBodyAIndex].velocity += impulse * solverBodies[contactConstraint.solverBodyAIndex].inverseMass;
		solverBodies[contactConstraint.solverBodyBIndex].velocity -= impulse * solverBodies[contactConstraint.solverBodyBIndex].inverseMass;

		// 角速度用の撃力計算
		impulse /= rotationCoefficient;

		solverBodies[contactConstraint.solverBodyAIndex].angularVelocity += solverBodies[contactConstraint.solverBodyAIndex].inverseInertiaTensor * Vector3::Cross(rA, impulse);
		solverBodies[contactConstraint.solverBodyBIndex].angularVelocity -= solverBodies[contactConstraint.solverBodyBIndex].inverseInertiaTensor * Vector3::Cross(rB, impulse);
	}
}


void CollisionSolverSystem::ReCalcPosRot()
{
	for (auto& constraint : contactConstraints)
	{
		// 変化した速度から位置を再計算
		// 位置 + Δv × 質量の逆数
		solverBodies[constraint.solverBodyAIndex].position = solverBodies[constraint.solverBodyAIndex].pastPos + solverBodies[constraint.solverBodyAIndex].velocity * ServiceLocator::GetTimeManager()->GetFixedDeltaTime() * solverBodies[constraint.solverBodyAIndex].inverseMass;
		// 位置 + Δv × 質量の逆数
		solverBodies[constraint.solverBodyBIndex].position = solverBodies[constraint.solverBodyBIndex].pastPos + solverBodies[constraint.solverBodyBIndex].velocity * ServiceLocator::GetTimeManager()->GetFixedDeltaTime() * solverBodies[constraint.solverBodyBIndex].inverseMass;
		
		// Δω
		Vector3 deltaAngularVelocity{ solverBodies[constraint.solverBodyAIndex].angularVelocity * ServiceLocator::GetTimeManager()->GetFixedDeltaTime() };
		// Δωの四元数を作る
		Quaternion rotOmega{ Quaternion::AngleAxis(deltaAngularVelocity.Length(),deltaAngularVelocity) };
		// 今の回転＋トルク(Δtに離散化)×慣性テンソルの逆行列
		solverBodies[constraint.solverBodyAIndex].rotation *= rotOmega;

		// Δω
		deltaAngularVelocity = solverBodies[constraint.solverBodyBIndex].angularVelocity * ServiceLocator::GetTimeManager()->GetFixedDeltaTime();
		// Δωの四元数を作る
		rotOmega = Quaternion::AngleAxis(deltaAngularVelocity.Length(), deltaAngularVelocity);
		// 今の回転＋トルク(Δtに離散化)×慣性テンソルの逆行列
		solverBodies[constraint.solverBodyBIndex].rotation *= rotOmega;
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

		// バネ定数
		float k{ 1.0f };
		// バネ定数×Δt
		float kDeltaTime{ k * ServiceLocator::GetTimeManager()->GetFixedDeltaTime() };
		// 減衰定数
		float c{ 1.0f };
		// biasを求める
		float erp{ kDeltaTime / (kDeltaTime + c) };
		float bias{ erp / ServiceLocator::GetTimeManager()->GetFixedDeltaTime() * constraint.penetration };

		// γ
		float gamma{ 1 / c + kDeltaTime };

		// 重心から衝突点ベクトル
		Vector3 rA{ constraint.position - solverBodies[constraint.solverBodyAIndex].position };
		Vector3 rB{ constraint.position - solverBodies[constraint.solverBodyBIndex].position };

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
		// λを求める λ = -Jv - bias / Jm^-1J^T
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

		// λ計算
		float lambda{ (jv - bias) / effectiveMass };

		// CFM適応
		lambda /= gamma;

		// 0未満にしない
		lambda = std::max(0.0f, lambda);

		// 速度の解消
		solverBodies[constraint.solverBodyAIndex].velocity -= constraint.normal * lambda * solverBodies[constraint.solverBodyAIndex].inverseMass;
		solverBodies[constraint.solverBodyAIndex].angularVelocity -= solverBodies[constraint.solverBodyAIndex].inverseInertiaTensor * rACross * lambda;

		solverBodies[constraint.solverBodyBIndex].velocity += constraint.normal * lambda * solverBodies[constraint.solverBodyBIndex].inverseMass;
		solverBodies[constraint.solverBodyBIndex].angularVelocity += solverBodies[constraint.solverBodyBIndex].inverseInertiaTensor * rBCross * lambda;

		// 位置の解消
		lambda = constraint.penetration / effectiveMass;

		// CFM適応
		// lambda /= gamma;

		solverBodies[constraint.solverBodyAIndex].position -= constraint.normal * lambda * solverBodies[constraint.solverBodyAIndex].inverseMass;
		solverBodies[constraint.solverBodyBIndex].position += constraint.normal * lambda * solverBodies[constraint.solverBodyBIndex].inverseMass;

		// 姿勢制御
		Vector3 angVec{ solverBodies[constraint.solverBodyAIndex].inverseInertiaTensor * rACross * lambda };
		Quaternion rotOmega{ Quaternion::AngleAxis(angVec.Length(), angVec) };
		solverBodies[constraint.solverBodyAIndex].rotation *= rotOmega;

		angVec = solverBodies[constraint.solverBodyAIndex].inverseInertiaTensor * rACross * lambda;
		rotOmega = Quaternion::AngleAxis(angVec.Length(), angVec);
		solverBodies[constraint.solverBodyAIndex].rotation *= rotOmega;
	}
}

void CollisionSolverSystem::End(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage)
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
	body.mass = _bodyStorage->mass[bodyIndex];
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
	body.position = _transformStorage->position[transformIndex];
	body.rotation = _transformStorage->rotation[transformIndex];
	body.mass = 0;
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
