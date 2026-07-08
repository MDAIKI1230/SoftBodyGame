#include "ConstraintBuildSystem.h"

void ConstraintBuildSystem::FixedUpdate(PhysicsTransformStorage* _transformStorage, ConstraintStorage* _constraintStorage, ConstraintBuffer* _constraintBuffer)
{
	BuildPointConstraint(_transformStorage, _constraintStorage, _constraintBuffer);
}

void ConstraintBuildSystem::BuildPointConstraint(PhysicsTransformStorage* _transformStorage, ConstraintStorage* _constraintStorage, ConstraintBuffer* _constraintBuffer)
{
	for (auto& constraint : _constraintStorage->pointConstraintStorage->constraints)
	{
		
	}
}
