#include "LinearTable.h"

void LinearTable::Reserve(uint32_t _size)
{
	slots.reserve(_size);
}

std::optional<uint32_t> LinearTable::Find(uint32_t _keyIndex) const
{
	if (slots.size() <= _keyIndex)
	{
		return std::nullopt;
	}

	if(slots[_keyIndex] == INVALID_DENSE_INDEX)
	{
		return std::nullopt;
	}

	return slots[_keyIndex];
}

void LinearTable::Set(uint32_t _keyIndex, uint32_t _denseIndex)
{
	if (slots.size() <= _keyIndex)
	{
		slots.resize(_keyIndex + 1, INVALID_DENSE_INDEX);
	}

	slots[_keyIndex] = _denseIndex;
}

void LinearTable::Erase(uint32_t _keyIndex)
{
	if (_keyIndex < slots.size())
	{
		slots[_keyIndex] = INVALID_DENSE_INDEX;
	}
}

void LinearTable::Clear()
{
	slots.clear();
}
