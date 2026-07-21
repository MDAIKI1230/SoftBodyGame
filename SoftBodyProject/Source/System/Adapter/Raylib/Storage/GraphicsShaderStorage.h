#pragma once

#include <vector>

#include <Raylib.h>

#include "GraphicsShaderHandle.h"

class GraphicsShaderStorage
{
public:
	// 持ってるか確認
	bool Has(GraphicsShaderHandle& _handle);
	// ハンドルが生きているか
	bool Alive(GraphicsShaderHandle& _handle);
	// 取得
	bool TryGet(GraphicsShaderHandle& _handle, Shader& _output);
	// 破棄
	void Remove(GraphicsShaderHandle& _handle);
	// 追加
	GraphicsShaderHandle Add(const Shader& _shader);
	// クリア
	void Clear();
private:
	std::vector<Shader> shaders;

	std::vector<uint32_t> slots;
	std::vector<uint32_t> generations;
	std::vector<uint32_t> freeSlots;
};
