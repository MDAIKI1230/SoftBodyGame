#pragma once

#include "ConstraintStorage.h"
#include "PhysicsTransformStorage.h"

class ConstraintDebugRenderingSystem
{
public:
	void Render(PhysicsTransformStorage* _transformStorage, ConstraintStorage* _constraintStorage);
private:
	// 点拘束描画
	void PointConstraintRender(PhysicsTransformStorage* _transformStorage, ConstraintStorage* _constraintStorage);
	// 距離拘束描画
	void DistanceConstraintRender(PhysicsTransformStorage* _transformStorage, ConstraintStorage* _constraintStorage);
};
