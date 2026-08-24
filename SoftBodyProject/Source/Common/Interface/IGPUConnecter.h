#pragma once

#include <cstdint>
#include <string>

#include "ComputeShaderHandle.h"
#include "GraphicsShaderHandle.h"
#include "ShaderBufferHandle.h"
#include "ShaderConstantBufferHandle.h"

class IGPUConnecter
{
public:
	// 初期化関数
	virtual void Initialize() = 0;

	// --- シェーダ関連-- -

	// コンピュートシェーダ読み込み
	virtual ComputeShaderHandle LoadComputeShader(const std::string& _filePath) = 0;
	// ディスパッチ
	virtual void Dispatch(ComputeShaderHandle _shader,uint32_t _groupX,uint32_t _groupY,uint32_t _groupZ) = 0;
	// シェーダ破棄
	virtual void DestroyComputeShader(ComputeShaderHandle _shader) = 0;


	// --- 描画系シェーダ関連 ---

	// 頂点シェーダ読み込み
	virtual GraphicsShaderHandle LoadVertexShader(const std::string& _filePath) = 0;
	// ピクセルシェーダ読み込み
	virtual GraphicsShaderHandle LoadPixelShader(const std::string& _filePath) = 0;
	// 頂点とピクセルシェーダ読み込み
	virtual GraphicsShaderHandle LoadGraphicsShader(const std::string& _vertexShaderFilePath, const std::string& _pixelShaderFilePath) = 0;
	// 描画関連(頂点とピクセル)シェーダスタート
	virtual void BeginGraphicsShader(GraphicsShaderHandle _shader) = 0;
	// 描画関連(頂点とピクセル)シェーダ終了
	virtual void EndGraphicsShader() = 0;
	// 描画関連(頂点とピクセル)シェーダ破棄
	virtual void DestroyGraphicsShader(GraphicsShaderHandle _shader) = 0;

	// --- バッファ関連 ---

	// バッファ作成
	virtual ShaderBufferHandle CreateShaderBuffer(uint32_t _size, const void* _initialData) = 0;
	// バッファ更新
	virtual void UpdateShaderBuffer(ShaderBufferHandle _buffer, const void* _data, uint32_t _size, uint32_t _offset = 0) = 0;
	// バッファバインド
	virtual void BindShaderBuffer(ShaderBufferHandle _buffer, uint32_t _binding) = 0;
	// バッファ値取り出し
	virtual void ReadShaderBuffer(ShaderBufferHandle _buffer, void* _destination, uint32_t _size, uint32_t _offset = 0) = 0;
	// バッファ破棄
	virtual void DestroyShaderBuffer(ShaderBufferHandle _buffer) = 0;

	// 定数バッファ作成
	virtual ShaderConstantBufferHandle CreateConstantBuffer(uint32_t _size) = 0;
	// 定数バッファにCPUからデータを書き込むためのアドレスを取得する関数
	virtual void* GetConstantBufferAddress(ShaderConstantBufferHandle _handle) = 0;
	// 定数バッファ更新
	virtual void UpdateConstantBuffer(ShaderConstantBufferHandle _handle, const void* _data, uint32_t _size) = 0;
	// 定数バッファバインド(頂点)
	virtual void BindConstantBufferVertex(ShaderConstantBufferHandle _handle, uint32_t _slot) = 0;
	// 定数バッファバインド(ピクセル)
	virtual void BindConstantBufferPixel(ShaderConstantBufferHandle _handle, uint32_t _slot) = 0;
	// 定数バッファ破棄
	virtual void DestroyConstantBuffer(ShaderConstantBufferHandle _buffer) = 0;

	// --- テクスチャ関連 ---
	// テクスチャをShaderに渡す。
	virtual void SetTexture(int _textureHandle, uint32_t _slot) = 0;

	// --- 同期 ---

	virtual void ShaderBufferBarrier() = 0;

	// 仮想デストラクタ
	virtual ~IGPUConnecter() = default;
};
