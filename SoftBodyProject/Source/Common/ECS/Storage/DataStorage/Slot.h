#pragma once

#include <stdint.h>

struct Slot
{
public:
    // コンストラクタ
    Slot(uint32_t _denseIndex) :
        denseIndex{ _denseIndex }
    {
    }

public:
    // 世代
    uint32_t generation{ 1 };
    // 生存フラグ
    bool alive{ true };

    // 実データの位置
    uint32_t denseIndex;
};
