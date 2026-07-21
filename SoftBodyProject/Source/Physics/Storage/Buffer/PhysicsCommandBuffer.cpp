#include "PhysicsCommandBuffer.h"

BodyUpdate& PhysicsCommandBuffer::Edit(BodyID& _id)
{
	SecureCapacity(_id.index);

	if (stamps[_id.index] == currentStamp)
	{
		return bodyUpdates[_id.index];
	}
	else
	{
		stamps[_id.index] = currentStamp;
		dirtyIndices.push_back(_id.index);
		return bodyUpdates[_id.index];
	}
}

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
