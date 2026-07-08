#pragma once

#include "ConstraintStorage.h"
#include "PhysicsTransformStorage.h"

#include "ConstraintBuffer.h"

class ConstraintBuildSystem
{
public:
	void FixedUpdate(PhysicsTransformStorage* _transformStorage, ConstraintStorage* _constraintStorage, ConstraintBuffer* _constraintBuffer);
private:
	void BuildPointConstraint(PhysicsTransformStorage* _transformStorage, ConstraintStorage* _constraintStorage, ConstraintBuffer* _constraintBuffer);
};
