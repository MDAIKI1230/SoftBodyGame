#include <Dxlib.h>

#include "DxlibGPUConnecter.h"

// 初期化関数
void DxlibGPUConnecter::Initialize()
{

}

// コンピュートシェーダ読み込み
ComputeShaderHandle DxlibGPUConnecter::LoadComputeShader(const std::string& _filePath)
{
	return {};
}
// ディスパッチ
void DxlibGPUConnecter::Dispatch(ComputeShaderHandle _shader, uint32_t _groupX, uint32_t _groupY, uint32_t _groupZ)
{

}
// シェーダ破棄
void DxlibGPUConnecter::DestroyComputeShader(ComputeShaderHandle _shader)
{

}

// 頂点シェーダ読み込み
GraphicsShaderHandle DxlibGPUConnecter::LoadVertexShader(const std::string& _filePath)
{
	DxlibGraphicsShaderHandles shader{ DxLib::LoadVertexShader(std::wstring(_filePath.begin(), _filePath.end()).c_str()),-1 };

	return graphicsShaderStorage.Add(shader);
}
// ピクセルシェーダ読み込み
GraphicsShaderHandle DxlibGPUConnecter::LoadPixelShader(const std::string& _filePath)
{
	DxlibGraphicsShaderHandles shader{ -1 ,DxLib::LoadPixelShader(std::wstring(_filePath.begin(), _filePath.end()).c_str()) };

	return graphicsShaderStorage.Add(shader);
}
// 頂点とピクセルシェーダ読み込み
GraphicsShaderHandle DxlibGPUConnecter::LoadGraphicsShader(const std::string& _vertexShaderFilePath, const std::string& _pixelShaderFilePath)
{
	DxlibGraphicsShaderHandles shader{
		DxLib::LoadVertexShader(std::wstring(_vertexShaderFilePath.begin(), _vertexShaderFilePath.end()).c_str()),
		DxLib::LoadPixelShader(std::wstring(_pixelShaderFilePath.begin(), _pixelShaderFilePath.end()).c_str()) };

	return graphicsShaderStorage.Add(shader);
}
// 描画関連(頂点とピクセル)シェーダスタート
void DxlibGPUConnecter::BeginGraphicsShader(GraphicsShaderHandle _shader)
{
	DxlibGraphicsShaderHandles shader;

	if(graphicsShaderStorage.TryGet(_shader,shader))
	{
		DxLib::MV1SetUseOrigShader(TRUE);

		DxLib::SetUseVertexShader(shader.vertexHandle);
		DxLib::SetUsePixelShader(shader.pixelShader);
	}
}
// 描画関連(頂点とピクセル)シェーダ終了
void DxlibGPUConnecter::EndGraphicsShader()
{
	DxLib::MV1SetUseOrigShader(FALSE);
}
// 描画関連(頂点とピクセル)シェーダ破棄
void DxlibGPUConnecter::DestroyGraphicsShader(GraphicsShaderHandle _shader)
{
	DxlibGraphicsShaderHandles shader;

	if (graphicsShaderStorage.TryGet(_shader, shader))
	{
		DxLib::DeleteShader(shader.vertexHandle);
		DxLib::DeleteShader(shader.pixelShader);

		graphicsShaderStorage.Remove(_shader);
	}
}

// バッファ作成
ShaderBufferHandle DxlibGPUConnecter::CreateShaderBuffer(uint32_t _size, const void* _initialData)
{
	return {};
}
// バッファ更新
void DxlibGPUConnecter::UpdateShaderBuffer(ShaderBufferHandle _buffer, const void* _data, uint32_t _size, uint32_t _offset)
{
	
}
// シェーダとバッファバインド
void DxlibGPUConnecter::BindShaderBuffer(ShaderBufferHandle _buffer, uint32_t _binding)
{
	
}
// バッファ値取り出し
void DxlibGPUConnecter::ReadShaderBuffer(ShaderBufferHandle _buffer, void* _destination, uint32_t _size, uint32_t _offset)
{

}
// バッファ破棄
void DxlibGPUConnecter::DestroyShaderBuffer(ShaderBufferHandle _buffer)
{
	
}

// 定数バッファ作成
ShaderConstantBufferHandle DxlibGPUConnecter::CreateConstantBuffer(uint32_t _size)
{
	return bufferStorage.Add(CreateShaderConstantBuffer(_size));
}
// 定数バッファにCPUからデータを書き込むためのアドレスを取得する関数
void* DxlibGPUConnecter::GetConstantBufferAddress(ShaderConstantBufferHandle _handle)
{
	int bufferHandle;

	if (bufferStorage.TryGet(_handle, bufferHandle))
	{
		return DxLib::GetBufferShaderConstantBuffer(bufferHandle);
	}

	return nullptr;
}
// 定数バッファ更新
void DxlibGPUConnecter::UpdateConstantBuffer(ShaderConstantBufferHandle _handle, const void* _data, uint32_t _size)
{
	int bufferHandle;

	if (bufferStorage.TryGet(_handle, bufferHandle))
	{
		void* destination{ GetBufferShaderConstantBuffer(bufferHandle) };
		memcpy(destination, _data, _size);
		UpdateShaderConstantBuffer(bufferHandle);
	}
}
// 定数バッファバインド(頂点)
void DxlibGPUConnecter::BindConstantBufferVertex(ShaderConstantBufferHandle _handle, uint32_t _slot)
{
	int bufferHandle;

	if (bufferStorage.TryGet(_handle, bufferHandle))
	{
		DxLib::SetShaderConstantBuffer(bufferHandle, DX_SHADERTYPE_VERTEX, _slot);
	}
}
// 定数バッファバインド(ピクセル)
void DxlibGPUConnecter::BindConstantBufferPixel(ShaderConstantBufferHandle _handle, uint32_t _slot)
{
	int bufferHandle;

	if (bufferStorage.TryGet(_handle, bufferHandle))
	{
		DxLib::SetShaderConstantBuffer(bufferHandle, DX_SHADERTYPE_PIXEL, _slot);
	}
}
// 定数バッファ破棄
void DxlibGPUConnecter::DestroyConstantBuffer(ShaderConstantBufferHandle _buffer)
{
	int bufferHandle;

	if (bufferStorage.TryGet(_buffer, bufferHandle))
	{
		DxLib::DeleteShaderConstantBuffer(bufferHandle);
		bufferStorage.Remove(_buffer);
	}
}

// テクスチャをShaderに渡す。
void DxlibGPUConnecter::SetTexture(int _textureHandle, uint32_t _slot)
{
	DxLib::SetUseTextureToShader(_slot, _textureHandle);
}

// --- 同期 ---
void DxlibGPUConnecter::ShaderBufferBarrier()
{
	
}
