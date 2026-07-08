#include "ConstraintSolverSystem.h"


//void CollisionSolverSystem::ConstraintSolver(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, CollisionManifoldBuffer* _manifoldBuffer)
//{
//	for (auto& constraint : constraints)
//	{
//		// 質量から両者がBodyを持っているかの判定をする(どちらかがBodyを持っているなら合計は0じゃないはず)
//		float totalInvMass{ solverBodies[constraint.solverBodyAIndex].inverseMass + solverBodies[constraint.solverBodyBIndex].inverseMass };
//		if (totalInvMass <= 0)
//		{
//			continue;
//		}
//
//		// biasを求める
//		const float bias{ ERP / ServiceLocator::GetTimeManager()->GetFixedDeltaTime() * constraint.constraintError };
//
//		// 変化量ベクトル
//		Vector3 deltaVector[4]{
//			solverBodies[constraint.solverBodyAIndex].velocity,
//			solverBodies[constraint.solverBodyAIndex].angularVelocity,
//			solverBodies[constraint.solverBodyBIndex].velocity,
//			solverBodies[constraint.solverBodyBIndex].angularVelocity,
//		};
//		// λを求める λ = -Jv - bias / M^-1
//		// ヤコビアンと変化量ベクトルから速度拘束条件Jv = 0のJv作成
//		float jv{ 0 };
//		for (int i{ 0 }; i < 4; i++)
//		{
//			jv += Vector3::Dot(constraint.jacobian[i], deltaVector[i]);
//		}
//
//		// 質量と慣性テンソルが速度に影響する度合い
//		float effectiveMass{
//			solverBodies[constraint.solverBodyAIndex].inverseMass +
//			Vector3::Dot(constraint.jacobian[1],solverBodies[constraint.solverBodyAIndex].inverseInertiaTensor * constraint.jacobian[1]) +
//			solverBodies[constraint.solverBodyBIndex].inverseMass +
//			Vector3::Dot(-constraint.jacobian[3],solverBodies[constraint.solverBodyBIndex].inverseInertiaTensor * -constraint.jacobian[3])
//		};
//
//		// λ計算(CFMも適応)
//		float lambda{ (jv + bias) / (effectiveMass + GAMMA) };
//
//		float oldLambda{ constraint.accumulatedLambda };
//
//		// 0未満にしない
//		constraint.accumulatedLambda = std::max(oldLambda + lambda, 0.0f);
//
//		float applyLambda{ constraint.accumulatedLambda - oldLambda };
//
//		// A速度の解消
//		solverBodies[constraint.solverBodyAIndex].velocity -= constraint.jacobian[0] * applyLambda * solverBodies[constraint.solverBodyAIndex].inverseMass;
//		solverBodies[constraint.solverBodyAIndex].angularVelocity -= solverBodies[constraint.solverBodyAIndex].inverseInertiaTensor * constraint.jacobian[1] * applyLambda;
//
//		// B速度の解消
//		solverBodies[constraint.solverBodyBIndex].velocity += constraint.jacobian[2] * applyLambda * solverBodies[constraint.solverBodyBIndex].inverseMass;
//		solverBodies[constraint.solverBodyBIndex].angularVelocity += solverBodies[constraint.solverBodyBIndex].inverseInertiaTensor * -constraint.jacobian[3] * applyLambda;
//	}
//}
//
