#pragma once

#include <cstdint>
#include <string>

#include "ComputeShaderHandle.h"
#include "GraphicsShaderHandle.h"
#include "ShaderBufferHandle.h"

class IGPUConnecter
{
public:
	// --- シェーダ関連-- -

	// コンピュートシェーダ読み込み
	virtual ComputeShaderHandle LoadComputeShader(const std::string& _filePath) = 0;
	// シェーダとバッファバインド
	virtual void BindShaderBuffer(ShaderBufferHandle& _buffer, uint32_t _binding) = 0;
	// ディスパッチ
	virtual void Dispatch(ComputeShaderHandle& _shader,uint32_t _groupX,uint32_t _groupY,uint32_t _groupZ) = 0;
	// シェーダ破棄
	virtual void DestroyComputeShader(ComputeShaderHandle& _shader) = 0;


	// --- 描画系シェーダ関連 ---

	// 頂点シェーダ読み込み
	virtual GraphicsShaderHandle LoadVertexShader(const std::string& _filePath) = 0;
	// ピクセルシェーダ読み込み
	virtual GraphicsShaderHandle LoadPixelShader(const std::string& _filePath) = 0;
	// 頂点とピクセルシェーダ読み込み
	virtual GraphicsShaderHandle LoadPixelShader(const std::string& _vertexShaderFilePath, const std::string& _pixelShaderFilePath) = 0;
	// シェーダとバッファバインド
	virtual void BindShaderBuffer(ShaderBufferHandle& _buffer, GraphicsShaderHandle _binding) = 0;
	// 描画関連(頂点とピクセル)シェーダスタート
	virtual void BeginGraphicsShader(GraphicsShaderHandle& _shader) = 0;
	// 描画関連(頂点とピクセル)シェーダ終了
	virtual void EndGraphicsShader() = 0;
	// 描画関連(頂点とピクセル)シェーダ破棄
	virtual void DestroyGraphicsShader(GraphicsShaderHandle& _shader) = 0;

	// --- バッファ関連 ---

	// バッファ作成
	virtual ShaderBufferHandle CreateShaderBuffer(uint32_t _size, const void* _initialData) = 0;
	// バッファ更新
	virtual void UpdateShaderBuffer(ShaderBufferHandle& _buffer, const void* _data, uint32_t _size, uint32_t _offset = 0) = 0;
	// バッファ値取り出し
	virtual void ReadShaderBuffer(ShaderBufferHandle& _buffer, void* _destination, uint32_t _size, uint32_t _offset = 0) = 0;
	// バッファ破棄
	virtual void DestroyShaderBuffer(ShaderBufferHandle& _buffer) = 0;

	// --- 同期 ---

	virtual void ShaderBufferBarrier() = 0;

	// 仮想デストラクタ
	virtual ~IGPUConnecter() = default;
};
