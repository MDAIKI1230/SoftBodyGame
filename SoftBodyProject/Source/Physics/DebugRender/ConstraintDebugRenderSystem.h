#pragma once

#include "ConstraintStorage.h"
#include "PhysicsTransformStorage.h"

class ConstraintDebugRenderSystem
{
public:
	void Render(ConstraintStorage* _constraintStorage, PhysicsTransformStorage* _transformStorage);
private:
	// 点拘束描画
	void PointConstraintRender(ConstraintStorage* _constraintStorage, PhysicsTransformStorage* _transformStorage);
	// 距離拘束描画
	void DistanceConstraintRender(ConstraintStorage* _constraintStorage, PhysicsTransformStorage* _transformStorage);
};
