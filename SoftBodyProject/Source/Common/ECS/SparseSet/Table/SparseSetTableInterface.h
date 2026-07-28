#pragma once

#include <cstdint>
#include <optional>

// KEYに対応したDenseIndexとの対応付けを行うクラス用
class SparseSetTableInterface
{
public:
	SparseSetTableInterface() = default;

	virtual void Reserve(uint32_t _size) = 0;
	virtual std::optional<uint32_t> Find(uint32_t _keyIndex) const = 0;
	virtual void Set(uint32_t _keyIndex, uint32_t _denseIndex) = 0;
	virtual void Erase(uint32_t _keyIndex) = 0;
	virtual void Clear() = 0;

	virtual ~SparseSetTableInterface() = default;
};
