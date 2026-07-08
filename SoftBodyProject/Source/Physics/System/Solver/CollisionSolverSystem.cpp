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

void CollisionSolverSystem::FixedUpdate(ColliderStorage* _colliderStorage, CollisionManifoldBuffer* _manifoldBuffer, SolverBodyBuffer* _solverBodyBuffer)
{
	// 解消準備
	StartUp(_colliderStorage, _manifoldBuffer, _solverBodyBuffer);
	// 速度解消を指定回数分回す
	for (int i{ 0 }; i < VELOCITY_SOLVER_TIMES; i++)
	{
		VelocitySolver(_manifoldBuffer, _solverBodyBuffer);
	}
	// 修正された速度で位置を再計算
	ReCalcPosRot(_solverBodyBuffer);
	// 位置/姿勢解消を指定回数分回す
	for (int i{ 0 }; i < POS_ROT_SOLVER_TIMES; i++)
	{
		PositionSolver(_manifoldBuffer, _solverBodyBuffer);
	}
	// 終了
	End(_manifoldBuffer);
}

void CollisionSolverSystem::StartUp(ColliderStorage* _colliderStorage, CollisionManifoldBuffer* _manifoldBuffer, SolverBodyBuffer* _solverBodyBuffer)
{
	// メモリの確保
	contactConstraints.reserve(_manifoldBuffer->manifolds.size() * 2);

	// すべての衝突情報から拘束条件とソルバ用Bodyの作成をする
	for (auto& manifold : _manifoldBuffer->manifolds)
	{
		for (int i{ 0 }; i < manifold.pointCount; i++)
		{
			ContactConstraint contactConstraint;
			// SolverBodyのIndexを取得
			PhysicsTransformID transformID{ _colliderStorage->GetTransformID(manifold.colliderA) };
			contactConstraint.solverBodyAIndex = _solverBodyBuffer->bodyMap[transformID];
			transformID = _colliderStorage->GetTransformID(manifold.colliderB);
			contactConstraint.solverBodyBIndex = _solverBodyBuffer->bodyMap[transformID];

			contactConstraint.positionA = manifold.points[i].positionA;
			contactConstraint.positionB = manifold.points[i].positionB;
			contactConstraint.normal = manifold.normal;
			contactConstraint.penetration = manifold.points[i].penetration;

			contactConstraints.push_back(contactConstraint);
		}
	}
}

void CollisionSolverSystem::VelocitySolver(CollisionManifoldBuffer* _manifoldBuffer, SolverBodyBuffer* _solverBodyBuffer)
{
	for (auto& constraint : contactConstraints)
	{
		// ボディA
		SolverBody& solverBodyA{ _solverBodyBuffer->solverBodies[constraint.solverBodyAIndex] };
		// ボディB
		SolverBody &solverBodyB{ _solverBodyBuffer->solverBodies[constraint.solverBodyBIndex] };
		// 質量から両者がBodyを持っているかの判定をする(どちらかがBodyを持っているなら合計は0じゃないはず)
		float totalInvMass{ solverBodyA.inverseMass + solverBodyB.inverseMass };
		if (totalInvMass <= 0)
		{
			continue;
		}

		// biasを求める
		const float bias{ ERP / ServiceLocator::GetTimeManager()->GetFixedDeltaTime() * constraint.penetration };

		// 重心から衝突点ベクトル
		Vector3 rA{ constraint.positionA - solverBodyA.position };
		Vector3 rB{ constraint.positionB - solverBodyB.position };

		// 重心から衝突点ベクトルAと法線の外積
		Vector3 rACross{ Vector3::Cross(rA,constraint.normal) };
		// 重心から衝突点ベクトルBと法線の外積
		Vector3 rBCross{ Vector3::Cross(rB,constraint.normal) };

		// ヤコビアン
		Vector3 jacobian[4]{ constraint.normal,rACross,-constraint.normal,-rBCross };
		// 変化量ベクトル
		Vector3 deltaVector[4]{
			solverBodyA.velocity,
			solverBodyA.angularVelocity,
			solverBodyB.velocity,
			solverBodyB.angularVelocity,
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
			solverBodyA.inverseMass +
			Vector3::Dot(rACross,solverBodyA.inverseInertiaTensor * rACross) +
			solverBodyB.inverseMass +
			Vector3::Dot(rBCross,solverBodyB.inverseInertiaTensor * rBCross)
		};

		// λ計算(CFMも適応)
		float lambda{ (jv + bias) / (effectiveMass + GAMMA) };

		float oldLambda{ constraint.accumulatedLambda };

		// 0未満にしない
		constraint.accumulatedLambda = std::max(oldLambda + lambda, 0.0f);

		float applyLambda{ constraint.accumulatedLambda - oldLambda };

		// A速度の解消
		solverBodyA.velocity -= constraint.normal * applyLambda * solverBodyA.inverseMass;
		solverBodyA.angularVelocity -= solverBodyA.inverseInertiaTensor * rACross * applyLambda;

		// B速度の解消
		solverBodyB.velocity += constraint.normal * applyLambda * solverBodyB.inverseMass;
		solverBodyB.angularVelocity += solverBodyB.inverseInertiaTensor * rBCross * applyLambda;

		// 摩擦
		FrictionSolver(solverBodyA, rA, solverBodyB, rB, constraint, effectiveMass);
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
	_bodyA.velocity -= tangent * applyLambda * _bodyA.inverseMass;
	_bodyA.angularVelocity -= _bodyA.inverseInertiaTensor * rACross * applyLambda;

	// B速度の摩擦
	_bodyB.velocity += tangent * applyLambda * _bodyB.inverseMass;
	_bodyB.angularVelocity += _bodyB.inverseInertiaTensor * rBCross * applyLambda;
}


void CollisionSolverSystem::ReCalcPosRot(SolverBodyBuffer* _solverBodyBuffer)
{
	for (auto& body : _solverBodyBuffer->solverBodies)
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

void CollisionSolverSystem::PositionSolver(CollisionManifoldBuffer* _manifoldBuffer, SolverBodyBuffer* _solverBodyBuffer)
{
	for (auto& constraint : contactConstraints)
	{
		// ボディA
		SolverBody& solverBodyA{ _solverBodyBuffer->solverBodies[constraint.solverBodyAIndex] };
		// ボディB
		SolverBody& solverBodyB{ _solverBodyBuffer->solverBodies[constraint.solverBodyBIndex] };
		// 質量から両者がBodyを持っているかの判定をする(どちらかがBodyを持っているなら合計は0じゃないはず)
		float totalInvMass{ solverBodyA.inverseMass + solverBodyB.inverseMass };
		if (totalInvMass <= 0)
		{
			continue;
		}

		// biasを求める
		float bias{ ERP / ServiceLocator::GetTimeManager()->GetFixedDeltaTime() * constraint.penetration };

		// 重心から衝突点ベクトル
		Vector3 rA{ constraint.positionA - solverBodyA.position };
		Vector3 rB{ constraint.positionB - solverBodyB.position };

		// 重心から衝突点ベクトルAと法線の外積
		Vector3 rACross{ Vector3::Cross(rA,constraint.normal) };
		// 重心から衝突点ベクトルBと法線の外積
		Vector3 rBCross{ Vector3::Cross(rB,constraint.normal) };

		// 質量と慣性テンソルが速度に影響する度合い
		float effectiveMass{
			solverBodyA.inverseMass +
			Vector3::Dot(rACross,solverBodyA.inverseInertiaTensor * rACross) +
			solverBodyB.inverseMass +
			Vector3::Dot(rBCross,solverBodyB.inverseInertiaTensor * rBCross)
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
		solverBodyA.position -= constraint.normal * applyLambda * solverBodyA.inverseMass;
		Vector3 angVec{ solverBodyA.inverseInertiaTensor * rACross * applyLambda };
		Quaternion rotOmega{ Quaternion::AngleAxis(angVec.Length(), -angVec) };
		solverBodyA.rotation *= rotOmega;

		// Bの位置/姿勢制御
		solverBodyB.position += constraint.normal * applyLambda * solverBodyB.inverseMass;
		angVec = solverBodyB.inverseInertiaTensor * rBCross * applyLambda;
		rotOmega = Quaternion::AngleAxis(angVec.Length(), angVec);
		solverBodyB.rotation *= rotOmega;
	}
}

void CollisionSolverSystem::End(CollisionManifoldBuffer* _manifoldBuffer)
{
	// リセット
	contactConstraints.clear();
	_manifoldBuffer->Clear();
}
