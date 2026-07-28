#include "HashTable.h"

void HashTable::Reserve(uint32_t _size)
{
    slots.reserve(_size);
}

std::optional<uint32_t> HashTable::Find(uint32_t _keyIndex) const
{
    const auto it = slots.find(_keyIndex);

    if (it == slots.end())
    {
        return std::nullopt;
    }

    return it->second;
}

void HashTable::Set(uint32_t _keyIndex, uint32_t _denseIndex)
{
    slots.insert_or_assign(_keyIndex, _denseIndex);
}

void HashTable::Erase(uint32_t _keyIndex)
{
    slots.erase(_keyIndex);
}

void HashTable::Clear()
{
    slots.clear();
}
