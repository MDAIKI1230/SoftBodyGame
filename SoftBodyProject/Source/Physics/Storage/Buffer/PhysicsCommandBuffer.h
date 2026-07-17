#pragma once

#include <vector>

class PhysicsCommandBuffer
{
public:
	// 記録開始
	void BeginRecord();
private:
	// メモリ確保
	void SecureCapacity(uint32_t _size);
private:
	uint32_t currentStamp{ 0 };

	std::vector<uint32_t> stamps;
	std::vector<uint32_t> dirtyIndices;
};
