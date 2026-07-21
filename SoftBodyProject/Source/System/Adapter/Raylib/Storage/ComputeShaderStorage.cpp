#include "ComputeShaderStorage.h"

// 持ってるか確認
bool ComputeShaderStorage::Has(ComputeShaderHandle& _handle)
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
bool ComputeShaderStorage::Alive(ComputeShaderHandle& _handle)
{
	if (generations.size() <= _handle.index)
	{
		return false;
	}

	return generations[_handle.index] == _handle.generation;
}
// 取得
bool ComputeShaderStorage::TryGet(ComputeShaderHandle& _handle, unsigned int& _output)
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
void ComputeShaderStorage::Remove(ComputeShaderHandle& _handle)
{
	if (Alive(_handle))
	{
		generations[_handle.index]++;
		freeSlots.push_back(_handle.index);
	}
}
// 追加
ComputeShaderHandle ComputeShaderStorage::Add(const unsigned int _shader)
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
	return ComputeShaderHandle{ index,generations[index] };
}
// クリア
void ComputeShaderStorage::Clear()
{
	shaders.clear();
	slots.clear();
	generations.clear();
	freeSlots.clear();
}
