#pragma once

#include "PhysicsCommandBuffer.h"
#include "BodyStorage.h"
#include "PhysicsTransformStorage.h"

class ApplyPhysicsCommandBufferSystem
{
public:
	// 適用関数
	void Apply(PhysicsCommandBuffer* _commandBuffer, PhysicsTransformStorage* _transformStorage, BodyStorage* _bodyStorage);
	
private:
	// リジッドボディの更新
	void ApplyBodyCommand(PhysicsCommandBuffer* _commandBuffer, PhysicsTransformStorage* _transformStorage, BodyStorage* _bodyStorage);
};
