#pragma once

#include <vector>

#include "../RaylibInclude.h"

#include "ComputeShaderHandle.h"


class ComputeShaderStorage
{
public:
	// 持ってるか確認
	bool Has(ComputeShaderHandle& _handle);
	// ハンドルが生きているか
	bool Alive(ComputeShaderHandle& _handle);
	// 取得
	bool TryGet(ComputeShaderHandle& _handle, unsigned int& _output);
	// 破棄
	void Remove(ComputeShaderHandle& _handle);
	// 追加
	ComputeShaderHandle Add(const unsigned int _shader);
	// クリア
	void Clear();
private:
	std::vector<unsigned int> shaders;

	std::vector<uint32_t> slots;
	std::vector<uint32_t> generations;
	std::vector<uint32_t> freeSlots;
};
