#pragma once

#include <vector>

#include "BodyUpdate.h"
#include "BodyID.h"

class PhysicsCommandBuffer
{
public:
	// 編集
	BodyUpdate& Edit(BodyID& _id);
	// 記録開始
	void BeginRecord();
private:
	// メモリ確保
	void SecureCapacity(uint32_t _size);
private:
	std::vector<BodyUpdate> bodyUpdates;

	uint32_t currentStamp{ 0 };

	std::vector<uint32_t> stamps;
	std::vector<uint32_t> dirtyIndices;
};
