#pragma once

#include <cstdint>
#include <vector>
#include <span>

template<class T>
class PhysicsBufferBase
{
public:
    // 新規作成
    template<class... Args>
    uint32_t Emplace(Args&&... args)
    {
        uint32_t index{ static_cast<uint32_t>(values.size()) };

        values.emplace_back(std::forward<Args>(args)...);

        return index;
    }
    // 追加
    virtual void Add(const T& _value)
    {
        values.push_back(_value);
    }
    // メモリ確保
    virtual void Reserve(uint32_t size)
    {
        values.reserve(size);
    }
    // 要素全削除
    virtual void Clear()
    {
        values.clear();
    }
    // サイズ取得
    virtual size_t GetSize() const
    {
        return values.size();
    }
    // 取得
    virtual const T& Get(uint32_t index) const
    {
        return values[index];
    }
    // 参照取得
    virtual T& Edit(uint32_t index)
    {
        return values[index];
    }
    // 全要素取得
    virtual std::span<const T> GetAll() const
    {
        return { values.data(), values.size() };
    }
    // 全要素参照取得
    virtual std::span<T> EditAll()
    {
        return { values.data(), values.size() };
    }

protected:
    std::vector<T> values;
};
