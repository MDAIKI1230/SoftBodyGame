#pragma once

#include <array>
#include <memory>
#include <vector>

#include "SparseSetTableInterface.h"

#include "GenerationalID.h"

template<uint32_t PAGE_SIZE = 4096>
class PageTable : public SparseSetTableInterface
{
public:
    // メモリ確保
    void Reserve(uint32_t _size) override
    {
        const uint32_t pageCount{ size / PAGE_SIZE + (size % PAGE_SIZE != 0) };

        pages.reserve(pageCount);
    }

    // 探す
    std::optional<uint32_t> Find(uint32_t _keyIndex) const override
    {
        // Page数とOffset計算
        const uint32_t pageIndex{ _keyIndex / PAGE_SIZE };
        const uint32_t offset{ _keyIndex % PAGE_SIZE };

        // サイズ判定と、Page存在チェック
        if (pageIndex >= pages.size() || pages[pageIndex] == nullptr)
        {
            return std::nullopt;
        }

        // 値持ってきて無効値じゃないなら答えを返す
        const uint32_t result{ pages[pageIndex]->slots[offset] };

        if (result == INVALID_DENSE_INDEX)
        {
            return std::nullopt;
        }

        return result;
    }

    // 新たに追加
    void Set(uint32_t _keyIndex, uint32_t _denseIndex) override
    {
        // Page数とOffset計算
        const uint32_t pageIndex{ _keyIndex / PAGE_SIZE };
        const uint32_t offset{ _keyIndex % PAGE_SIZE };

        // Pageサイズチェック
        if (pages.size() <= pageIndex)
        {
            pages.resize(pageIndex + 1);
        }

        // 存在チェック
        if (pages[pageIndex] == nullptr)
        {
            pages[pageIndex] = std::make_unique<Page>();
        }

        // 追加
        pages[pageIndex]->slots[offset] = _denseIndex;
    }

    // 削除
    void Erase(uint32_t _keyIndex) override
    {
        // Page数とOffset計算
        const uint32_t pageIndex{ _keyIndex / PAGE_SIZE };
        const uint32_t offset{ _keyIndex % PAGE_SIZE };

        // チェックして対応地に無効値を代入
        if (pageIndex < pages.size() && pages[pageIndex])
        {
            pages[pageIndex]->slots[offset] = INVALID_DENSE_INDEX;
        }
    }

    // 全要素なくす。
    void Clear() override
    {
        pages.clear();
    }

private:
    static constexpr uint32_t INVALID_DENSE_INDEX{ GenerationalID::INVALID_INDEX };

    struct Page
    {
        Page()
        {
            slots.fill(INVALID_DENSE_INDEX);
        }

        std::array<uint32_t, PAGE_SIZE> slots;
    };

    std::vector<std::unique_ptr<Page>> pages;
};
