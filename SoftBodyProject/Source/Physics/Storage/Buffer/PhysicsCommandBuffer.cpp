#include "PhysicsCommandBuffer.h"

void PhysicsCommandBuffer::BeginRecord()
{
	currentStamp++;
	dirtyIndices.clear();
}

void PhysicsCommandBuffer::SecureCapacity(uint32_t _size)
{
	stamps.reserve(_size);
	bodyUpdates.reserve(_size);
}
