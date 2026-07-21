#pragma once

#include <vector>

template<typename HANDLE,typename DATA>
class RaylibStorage
{
public:
	// 持ってるか確認
	bool Has(HANDLE& _handle)
	{
		if (!Alive(_handle))
		{
			return false;
		}

		uint32_t index{ slots[_handle.index] };
		if (shaders.size() <= index)
		{
			return false;
		}

		if (generations[index] != _handle.generation)
		{
			return false;
		}

		return true;
	}
	// ハンドルが生きているか
	bool Alive(HANDLE& _handle)
	{
		if (generations.size() <= _handle.index)
		{
			return false;
		}

		return generations[_handle.index] == _handle.generation;
	}
	// 取得
	bool TryGet(HANDLE& _handle, DATA& _output)
	{
		if (!Alive(_handle))
		{
			return false;
		}

		uint32_t index{ slots[_handle.index] };
		if (shaders.size() <= index)
		{
			return false;
		}

		if (generations[index] != _handle.generation)
		{
			return false;
		}

		_output = shaders[index];

		return true;
	}
	// 破棄
	void Remove(HANDLE& _handle)
	{
		if (Alive(_handle))
		{
			generations[_handle.index]++;
			freeSlots.push_back(_handle.index);
		}
	}
	// 追加
	HANDLE Add(const DATA& _shader)
	{
		// ハンドルのIndexを決定
		uint32_t index;
		if (freeSlots.empty())
		{
			index = static_cast<uint32_t>(slots.size());
			// 枠増加
			slots.push_back(static_cast<uint32_t>(shaders.size()));
			// 世代を追加
			generations.emplace_back(1);
		}
		else
		{
			index = freeSlots.back();
			freeSlots.pop_back();
			// 枠に入れとく
			slots[index] = static_cast<uint32_t>(shaders.size());
		}

		// Shaderを追加
		shaders.push_back(_shader);
		// 世代とIndexからHandle出す
		return HANDLE{ index,generations[index] };
	}
	// クリア
	void Clear()
	{
		shaders.clear();
		slots.clear();
		generations.clear();
		freeSlots.clear();
	}

private:
	std::vector<DATA> shaders;

	std::vector<uint32_t> slots;
	std::vector<uint32_t> generations;
	std::vector<uint32_t> freeSlots;
};
