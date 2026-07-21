#pragma once

#include <vector>

#include "RaylibInclude.h"

#include "IGPUConnecter.h"

class RaylibGPUConnecter :public IGPUConnecter
{
	// --- シェーダ関連-- -

	// コンピュートシェーダ読み込み
	ComputeShaderHandle LoadComputeShader(const std::string& _shaderPath) override;
	// シェーダとバッファバインド
	void BindShaderBuffer(ShaderBufferHandle& _buffer, uint32_t _binding) override;
	// ディスパッチ
	void Dispatch(ComputeShaderHandle& _shader, uint32_t _groupX, uint32_t _groupY, uint32_t _groupZ) override;
	// シェーダ破棄
	void DestroyComputeShader(ComputeShaderHandle& _shader) override;


	// --- 描画系シェーダ関連 ---

	// 頂点シェーダ読み込み
	GraphicsShaderHandle LoadVertexShader(const std::string& _vertexShaderPath) override;
	// ピクセルシェーダ読み込み
	GraphicsShaderHandle LoadPixelShader(const std::string& _pixelShaderPath) override;
	// シェーダとバッファバインド
	void BindShaderBuffer(ShaderBufferHandle& _buffer, uint32_t _binding) override;
	// 描画関連(頂点とピクセル)シェーダスタート
	void BeginGraphicsShader(GraphicsShaderHandle& _shader) override;
	// 描画関連(頂点とピクセル)シェーダ終了
	void EndGraphicsShader() override;
	// 描画関連(頂点とピクセル)シェーダ破棄
	void DestroyGraphicsShader(GraphicsShaderHandle& _shader) override;

	// --- バッファ関連 ---

	// バッファ作成
	ShaderBufferHandle CreateShaderBuffer(size_t _size, const void* _initialData) override;
	// バッファ更新
	void UpdateShaderBuffer(ShaderBufferHandle& _buffer, const void* _data, size_t _size, size_t _offset = 0) override;
	// バッファ値取り出し
	void ReadShaderBuffer(ShaderBufferHandle& _buffer, void* _destination, size_t _size, size_t _offset = 0) override;
	// バッファ破棄
	void DestroyShaderBuffer(ShaderBufferHandle& _buffer) override;

	// --- 同期 ---

	void ShaderBufferBarrier() override;

private:
	std::vector<Shader> shaders;

	std::vector<uint32_t> slots;
	std::vector<size_t> freeSlots;
};
