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
VertexShaderHandle DxlibGPUConnecter::LoadVertexShader(const std::string& _filePath)
{
	int nativeHandle{ DxLib::LoadVertexShader(std::wstring(_filePath.begin(), _filePath.end()).c_str()) };

	if (nativeHandle == -1)
	{
		return {};
	}

	return vertexShaderStorage.Add(nativeHandle);
}
// ピクセルシェーダ読み込み
PixelShaderHandle DxlibGPUConnecter::LoadPixelShader(const std::string& _filePath)
{
	int nativeHandle{ DxLib::LoadPixelShader(std::wstring(_filePath.begin(), _filePath.end()).c_str()) };

	if (nativeHandle == -1)
	{
		return {};
	}

	return pixelShaderStorage.Add(nativeHandle);
}

// 頂点シェーダスタート
void DxlibGPUConnecter::BeginVertexShader(VertexShaderHandle _shader)
{
	DxLib::MV1SetUseOrigShader(TRUE);

	int nativeHandle;
	if (vertexShaderStorage.TryGet(_shader, nativeHandle))
	{
		DxLib::SetUseVertexShader(nativeHandle);
	}
}
// ピクセルシェーダスタート
void DxlibGPUConnecter::BeginPixelShader(PixelShaderHandle _shader)
{
	DxLib::MV1SetUseOrigShader(TRUE);

	int nativeHandle;
	if (pixelShaderStorage.TryGet(_shader, nativeHandle))
	{
		DxLib::SetUsePixelShader(nativeHandle);
	}
}
// 描画関連(頂点とピクセル)シェーダ終了
void DxlibGPUConnecter::EndGraphicsShader()
{
	DxLib::MV1SetUseOrigShader(FALSE);
}

// 頂点シェーダ破棄
void DxlibGPUConnecter::DestroyVertexShader(VertexShaderHandle _shader)
{
	int nativeHandle;
	if (vertexShaderStorage.TryGet(_shader, nativeHandle))
	{
		DxLib::DeleteShader(nativeHandle);
	}
}
// ピクセルシェーダ破棄
void DxlibGPUConnecter::DestroyPixelShader(PixelShaderHandle _shader)
{
	int nativeHandle;
	if (pixelShaderStorage.TryGet(_shader, nativeHandle))
	{
		DxLib::DeleteShader(nativeHandle);
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

// --- 同期 ---
void DxlibGPUConnecter::ShaderBufferBarrier()
{
	
}

// すべてのリソースの削除
void DxlibGPUConnecter::DeleteAll()
{
	DxLib::InitShader();
}
