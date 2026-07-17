#pragma once

#include <vector>

#include "BodyUpdate.h"

class PhysicsCommandBuffer
{
public:
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
