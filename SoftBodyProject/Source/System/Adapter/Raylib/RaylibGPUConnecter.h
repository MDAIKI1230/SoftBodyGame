#pragma once

#include "IGPUConnecter.h"

#include "RaylibInclude.h"

#include "ResourceStorage.h"
#include "Struct/RaylibShaderBufferResource.h"

class RaylibGPUConnecter :public IGPUConnecter
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

	// 定数バッファ作成
	ShaderConstantBufferHandle CreateConstantBuffer(uint32_t _size) override;
	// 定数バッファ更新
	void UpdateConstantBuffer(ShaderConstantBufferHandle _handle, const void* _data, uint32_t _size) override;
	// 定数バッファバインド(頂点)
	void BindConstantBufferVertex(ShaderConstantBufferHandle _handle, uint32_t _slot) override;
	// 定数バッファバインド(ピクセル)
	void BindConstantBufferPixel(ShaderConstantBufferHandle _handle, uint32_t _slot) override;
	// 定数バッファ破棄
	void DestroyConstantBuffer(ShaderConstantBufferHandle _buffer);
	// 定数バッファにCPUからデータを書き込むためのアドレスを取得する関数
	void* GetConstantBufferAddress(ShaderConstantBufferHandle _handle) override;



	// --- 同期 ---

	void ShaderBufferBarrier() override;

private:
	static constexpr int SHADER_STORAGE_BARRIER_BIT{ 0x00002000 };

private:
	// 描画系シェーダストレージ
	ResourceStorage<VertexShaderHandle, Shader> graphicsShaderStorage;
	// コンピュートシェーダストレージ
	ResourceStorage<ComputeShaderHandle, unsigned int> computeShaderStorage;
	// バッファストレージ
	ResourceStorage<ShaderBufferHandle, RaylibShaderBufferResource> bufferStorage;
	// バッファストレージ
	ResourceStorage<ShaderConstantBufferHandle, RaylibShaderBufferResource> constantBufferStorage;

	// Windowsのみの奴やで―
	using MemoryBarrierFunction = void(__stdcall*)(unsigned int);

	MemoryBarrierFunction memoryBarrierFunction{ nullptr };

};
