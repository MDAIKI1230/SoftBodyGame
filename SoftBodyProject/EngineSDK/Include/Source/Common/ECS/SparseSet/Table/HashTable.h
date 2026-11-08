#pragma once

#include <unordered_map>

#include "SparseSetTableInterface.h"

class HashTable : public SparseSetTableInterface
{
public:
    // メモリ確保
    void Reserve(uint32_t _size) override;
    // 探す
    std::optional<uint32_t> Find(uint32_t _keyIndex) const override;
    // 新たに追加
    void Set(uint32_t _keyIndex, uint32_t _denseIndex) override;
    // 削除
    void Erase(uint32_t _keyIndex) override;
    // 全要素なくす。
    void Clear() override;

private:
    std::unordered_map<uint32_t, uint32_t> slots;
};
