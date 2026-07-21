#pragma once

#include <cstdint>
#include <string>

#include "ComputeShaderHandle.h"
#include "GraphicsShaderHandle.h"
#include "ShaderBufferHandle.h"

class IGPUConnecter
{
	// --- シェーダ関連-- -

	// コンピュートシェーダ読み込み
	virtual ComputeShaderHandle LoadComputeShader(const std::string& _shaderPath) = 0;
	// シェーダとバッファバインド
	virtual void BindShaderBuffer(ShaderBufferHandle& _buffer,uint32_t _binding) = 0;
	// ディスパッチ
	virtual void Dispatch(ComputeShaderHandle& _shader,uint32_t _groupX,uint32_t _groupY,uint32_t _groupZ) = 0;
	// シェーダ破棄
	virtual void DestroyComputeShader(ComputeShaderHandle& _shader) = 0;


	// --- 描画系シェーダ関連 ---

	// 頂点シェーダ読み込み
	virtual GraphicsShaderHandle LoadVertexShader(const std::string& _vertexShaderPath) = 0;
	// ピクセルシェーダ読み込み
	virtual GraphicsShaderHandle LoadPixelShader(const std::string& pixelShaderPath) = 0;
	// シェーダとバッファバインド
	virtual void BindShaderBuffer(ShaderBufferHandle& buffer, uint32_t binding) = 0;
	// 描画関連(頂点とピクセル)シェーダスタート
	virtual void BeginGraphicsShader(GraphicsShaderHandle& shader) = 0;
	// 描画関連(頂点とピクセル)シェーダ終了
	virtual void EndGraphicsShader() = 0;
	// 描画関連(頂点とピクセル)シェーダ破棄
	virtual void DestroyGraphicsShader(GraphicsShaderHandle& shader) = 0;

	// --- バッファ関連 ---

	// バッファ作成
	virtual ShaderBufferHandle CreateShaderBuffer(size_t size, const void* initialData) = 0;
	// バッファ更新
	virtual void UpdateShaderBuffer(ShaderBufferHandle& buffer, const void* data, size_t size, size_t offset = 0) = 0;
	// バッファ値取り出し
	virtual void ReadShaderBuffer(ShaderBufferHandle& buffer, void* destination, size_t size, size_t offset = 0) = 0;
	// バッファ破棄
	virtual void DestroyShaderBuffer(ShaderBufferHandle& buffer) = 0;

	// --- 同期 ---

	virtual void ShaderBufferBarrier() = 0;
};
