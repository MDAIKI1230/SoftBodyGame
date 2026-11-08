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
	// ヒンジ拘束描画
	void HingeConstraintRender(PhysicsTransformStorage* _transformStorage, ConstraintStorage* _constraintStorage);
	// 角度制限付き点拘束描画
	void AngleLimitPointConstraintRender(PhysicsTransformStorage* _transformStorage, ConstraintStorage* _constraintStorage);
	// 角度制限付きヒンジ拘束描画
	void AngleLimitHingeConstraintRender(PhysicsTransformStorage* _transformStorage, ConstraintStorage* _constraintStorage);
	// SwingTwist拘束描画
	void LimitedBallJointConstraintRender(PhysicsTransformStorage* _transformStorage, ConstraintStorage* _constraintStorage);
	// ある点を線上に制限する拘束描画
	void PointOnLineConstraintRender(PhysicsTransformStorage* _transformStorage, ConstraintStorage* _constraintStorage);
	// スライダー拘束描画
	void SliderConstraintRender(PhysicsTransformStorage* _transformStorage, ConstraintStorage* _constraintStorage);
	// 距離制限付きで点を線上に制限する拘束描画
	void LimitedPointOnLineConstraintRender(PhysicsTransformStorage* _transformStorage, ConstraintStorage* _constraintStorage);
	// 距離制限付きスライダー拘束描画
	void LimitedSliderConstraintRender(PhysicsTransformStorage* _transformStorage, ConstraintStorage* _constraintStorage);
};
