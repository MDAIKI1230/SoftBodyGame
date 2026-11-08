#pragma once

#include <vector>

#include "SparseSetTableInterface.h"

class LinearTable :public SparseSetTableInterface
{

public:
	void Reserve(uint32_t _size) override;
	std::optional<uint32_t> Find(uint32_t _keyIndex) const override;
	void Set(uint32_t _keyIndex, uint32_t _denseIndex) override;
	void Erase(uint32_t _key) override;
	void Clear() override;
private:
	static constexpr uint32_t INVALID_DENSE_INDEX{ UINT32_MAX };
private:
	std::vector<uint32_t> slots;
};
