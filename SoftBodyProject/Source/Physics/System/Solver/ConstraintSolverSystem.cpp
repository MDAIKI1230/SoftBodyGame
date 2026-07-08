#include "ServiceLocator.h"

#include "ConstraintSolverSystem.h"

ConstraintSolverSystem::ConstraintSolverSystem() :
	K{ 0.4f },
	K_DELTA_TIME{ K * ServiceLocator::GetTimeManager()->GetFixedDeltaTime() },
	C{ 0.4f },
	ERP{ K_DELTA_TIME / (K_DELTA_TIME + C) },
	GAMMA{ 1 / (C + K_DELTA_TIME) }
{
}

void ConstraintSolverSystem::ConstraintSolver(SolverBodyBuffer* _solverBodyBuffer, ConstraintBuffer* _constraintBuffer)
{
	for (auto& constraint : _constraintBuffer->constraints)
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
		const float bias{ ERP / ServiceLocator::GetTimeManager()->GetFixedDeltaTime() * constraint.constraintError };

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
			jv += Vector3::Dot(constraint.jacobian[i], deltaVector[i]);
		}

		// 質量と慣性テンソルが速度に影響する度合い
		float effectiveMass{
			solverBodyA.inverseMass +
			Vector3::Dot(constraint.jacobian[1],solverBodyA.inverseInertiaTensor * constraint.jacobian[1]) +
			solverBodyB.inverseMass +
			Vector3::Dot(-constraint.jacobian[3],solverBodyB.inverseInertiaTensor * -constraint.jacobian[3])
		};

		// λ計算(CFMも適応)
		float lambda{ (jv + bias) / (effectiveMass + GAMMA) };

		float oldLambda{ constraint.accumulatedLambda };

		// 0未満にしない
		constraint.accumulatedLambda = std::max(oldLambda + lambda, 0.0f);

		float applyLambda{ constraint.accumulatedLambda - oldLambda };

		// A速度の解消
		solverBodyA.velocity -= constraint.jacobian[0] * applyLambda * solverBodyA.inverseMass;
		solverBodyA.angularVelocity -= solverBodyA.inverseInertiaTensor * constraint.jacobian[1] * applyLambda;

		// B速度の解消
		solverBodyB.velocity += constraint.jacobian[2] * applyLambda * solverBodyB.inverseMass;
		solverBodyB.angularVelocity += solverBodyB.inverseInertiaTensor * -constraint.jacobian[3] * applyLambda;
	}
}

