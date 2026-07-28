#pragma once

#include <memory>

#include "IGPUConnecter.h"

#include "RaylibInclude.h"

#include "Storage/RaylibStorage.h"

class RaylibGPUConnecter :public IGPUConnecter
{
public:
	// コンストラクタ
	RaylibGPUConnecter();

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
	GraphicsShaderHandle LoadPixelShader(const std::string& _vertexShaderFilePath, const std::string& _pixelShaderFilePath) override;
	// 描画関連(頂点とピクセル)シェーダスタート
	void BeginGraphicsShader(GraphicsShaderHandle _shader) override;
	// 描画関連(頂点とピクセル)シェーダ終了
	void EndGraphicsShader() override;
	// 描画関連(頂点とピクセル)シェーダ破棄
	void DestroyGraphicsShader(GraphicsShaderHandle _shader) override;

	// --- バッファ関連 ---

	// バッファ作成
	ShaderBufferHandle CreateShaderBuffer(uint32_t _size, const void* _initialData) override;
	// バッファ更新
	void UpdateShaderBuffer(ShaderBufferHandle _buffer, const void* _data, uint32_t _size, uint32_t _offset = 0) override;
	// シェーダとバッファバインド
	void BindShaderBuffer(ShaderBufferHandle _buffer, uint32_t _binding) override;
	// バッファ値取り出し
	void ReadShaderBuffer(ShaderBufferHandle _buffer, void* _destination, uint32_t _size, uint32_t _offset = 0) override;
	// バッファ破棄
	void DestroyShaderBuffer(ShaderBufferHandle _buffer) override;

	// --- 同期 ---

	void ShaderBufferBarrier() override;

private:
	static constexpr int SHADER_STORAGE_BARRIER_BIT{ 0x00002000 };

private:
	// 描画系シェーダストレージ
	std::unique_ptr<RaylibStorage<GraphicsShaderHandle, Shader>> graphicsShaderStorage;
	// コンピュートシェーダストレージ
	std::unique_ptr<RaylibStorage<ComputeShaderHandle, unsigned int>> computeShaderStorage;
	// バッファストレージ
	std::unique_ptr<RaylibStorage<ShaderBufferHandle, unsigned int>> bufferStorage;

	// Windowsのみの奴やで―
	using MemoryBarrierFunction = void(__stdcall*)(unsigned int);

	MemoryBarrierFunction memoryBarrierFunction{ nullptr };

};
