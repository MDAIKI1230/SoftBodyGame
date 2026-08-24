#pragma once

#include "IGPUConnecter.h"

#include "ResourceStorage.h"
#include "Struct/DxlibGraphicsShaderHandles.h"
#include "Struct/DxlibShaderBufferResource.h"

class DxlibGPUConnecter :public IGPUConnecter
{
public:
	// 初期化関数
	void Initialize() override;

	// --- シェーダ関連-- -

	// コンピュートシェーダ読み込み
	ComputeShaderHandle LoadComputeShader(const std::string& _filePath) override;
	// ディスパッチ
	void Dispatch(ComputeShaderHandle _shader, uint32_t _groupX, uint32_t _groupY, uint32_t _groupZ) override;
	// シェーダ破棄
	void DestroyComputeShader(ComputeShaderHandle _shader) override;


	// --- 描画系シェーダ関連 ---

	// 頂点シェーダ読み込み
	GraphicsShaderHandle LoadVertexShader(const std::string& _filePath) override;
	// ピクセルシェーダ読み込み
	GraphicsShaderHandle LoadPixelShader(const std::string& _filePath) override;
	// 頂点とピクセルシェーダ読み込み
	GraphicsShaderHandle LoadGraphicsShader(const std::string& _vertexShaderFilePath, const std::string& _pixelShaderFilePath) override;
	// 描画関連(頂点とピクセル)シェーダスタート
	void BeginGraphicsShader(GraphicsShaderHandle _shader) override;
	// 描画関連(頂点とピクセル)シェーダ終了
	void EndGraphicsShader() override;
	// 描画関連(頂点とピクセル)シェーダ破棄
	void DestroyGraphicsShader(GraphicsShaderHandle _shader) override;

	// --- バッファ関連 ---

	// バッファ作成
	ShaderBufferHandle CreateShaderBuffer(uint32_t _size, const void* _initialData) override;
	// 定数バッファにCPUからデータを書き込むためのアドレスを取得する関数
	void* GetConstantBufferAddress(ShaderConstantBufferHandle _handle) override;
	// バッファ更新
	void UpdateShaderBuffer(ShaderBufferHandle _buffer, const void* _data, uint32_t _size, uint32_t _offset = 0) override;
	// シェーダとバッファバインド
	void BindShaderBuffer(ShaderBufferHandle _buffer, uint32_t _binding) override;
	// バッファ値取り出し
	void ReadShaderBuffer(ShaderBufferHandle _buffer, void* _destination, uint32_t _size, uint32_t _offset = 0) override;
	// バッファ破棄
	void DestroyShaderBuffer(ShaderBufferHandle _buffer) override;

	// 定数バッファ作成
	ShaderConstantBufferHandle CreateConstantBuffer(uint32_t _size);
	// 定数バッファ更新
	void UpdateConstantBuffer(ShaderConstantBufferHandle _handle, const void* _data, uint32_t _size);
	// 定数バッファバインド(頂点)
	void BindConstantBufferVertex(ShaderConstantBufferHandle _handle, uint32_t _slot);
	// 定数バッファバインド(ピクセル)
	void BindConstantBufferPixel(ShaderConstantBufferHandle _handle, uint32_t _slot);
	// 定数バッファ破棄
	void DestroyConstantBuffer(ShaderConstantBufferHandle _buffer);

	// --- テクスチャ関連 ---
	// テクスチャをShaderに渡す。
	void SetTexture(int _textureHandle, uint32_t _slot) override;

	// --- 同期 ---

	void ShaderBufferBarrier() override;
private:
	// 描画系シェーダストレージ
	ResourceStorage<GraphicsShaderHandle, DxlibGraphicsShaderHandles> graphicsShaderStorage;
	// コンピュートシェーダストレージ
	ResourceStorage<ComputeShaderHandle, int> computeShaderStorage;
	// バッファストレージ
	ResourceStorage<ShaderConstantBufferHandle, int> bufferStorage;
};
