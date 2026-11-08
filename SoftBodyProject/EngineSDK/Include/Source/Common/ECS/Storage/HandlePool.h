#pragma once

#include <vector>

template<class HANDLE>
class HandlePool
{
public:
	HANDLE Acquire()
	{
		// ハンドルのIndexを決定
		uint32_t index;
		if (freeSlots.empty())
		{
			index = static_cast<uint32_t>(generations.size());
			// 世代を追加
			generations.emplace_back(1);
		}
		else
		{
			index = freeSlots.back();
			freeSlots.pop_back();
		}

		// 世代とIndexからHandle出す
		return HANDLE{ index,generations[index] };
	}
	bool IsAlive(HANDLE _handle) const
	{
		if (generations.size() <= _handle.GetIndex())
		{
			return false;
		}

		return generations[_handle.GetIndex()] == _handle.GetGeneration();
	}
	void Release(HANDLE _handle)
	{
		if (IsAlive(_handle))
		{
			generations[_handle.GetIndex()]++;
			freeSlots.push_back(_handle.GetIndex());
		}
	}
	void Clear()
	{
		freeSlots.clear();
		freeSlots.reserve(generations.size());

		for (uint32_t i = 0; i < generations.size(); ++i)
		{
			++generations[i];

			if (generations[i] == HANDLE::INVALID_GENERATION)
			{
				++generations[i];
			}

			freeSlots.push_back(i);
		}
	}
private:
	std::vector<uint32_t> generations;
	std::vector<uint32_t> freeSlots;
};
