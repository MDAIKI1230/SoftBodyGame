#pragma once

#include <vector>
#include <cstdint>

#include "Slot.h"

template<class ID>
class SlotPool
{
public:
    // 新たにスロットを作成
    ID Create(uint32_t _denseIndex)
    {
        // フリーリストからチェック
        if (freeSlots.empty())
        {
            uint32_t index{ static_cast<uint32_t>(slots.size()) };

            slots.emplace_back(_denseIndex);

            auto& slot = slots.back();
            slot.alive = true;

            return ID{ index, slot.generation };
        }
        // フリースロットから値をとってきてスロットを作る
        uint32_t index{ freeSlots.back() };
        freeSlots.pop_back();

        uint32_t generation{ slots[index].generation };

        slots[index] = Slot{ _denseIndex };
        slots[index].generation = generation;
        slots[index].alive = true;

        return ID{ index, generation };
    }

    // スロットの解放
    void Release(ID id)
    {
        Slot& slot = slots[id.GetIndex()];

        // 生存フラグを折る
        slot.alive = false;
        // 世代増加
        slot.generation++;
        // フリーリストに追加
        freeSlots.push_back(id.GetIndex());
    }

    // そのIDが生きているか
    bool IsAlive(ID id) const
    {
        // IDの生存チェック＋サイズないかのチェック
        if (!id.IsValid() || id.GetIndex() >= slots.size())
        {
            return false;
        }

        const Slot& slot = slots[id.GetIndex()];
        // 世代とかも確認しつつ
        return slot.alive && slot.generation == id.GetGeneration();
    }

    // スロット参照取得
    Slot& Edit(ID id)
    {
        return slots[id.GetIndex()];
    }

    // スロット取得
    const Slot& Get(ID id) const
    {
        return slots[id.GetIndex()];
    }

private:
    std::vector<Slot> slots;
    std::vector<uint32_t> freeSlots;
};
