#include "PhysicsCommandBuffer.h"

BodyUpdate& PhysicsCommandBuffer::Edit(BodyID& _id)
{
	SecureCapacity(_id.GetIndex());

	if (stamps[_id.GetIndex()] == currentStamp)
	{
		return bodyUpdates[_id.GetIndex()];
	}
	else
	{
		stamps[_id.GetIndex()] = currentStamp;
		dirtyIndices.push_back(_id.GetIndex());
		return bodyUpdates[_id.GetIndex()];
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
