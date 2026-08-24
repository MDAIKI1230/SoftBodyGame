#include "RaylibGPUConnecter.h"

// 初期化関数
void RaylibGPUConnecter::Initialize()
{
	// メモリバリア用の関数取得
	memoryBarrierFunction = reinterpret_cast<MemoryBarrierFunction>(::rlGetProcAddress("glMemoryBarrier"));
}

// --- シェーダ関連-- -

// コンピュートシェーダ読み込み
ComputeShaderHandle RaylibGPUConnecter::LoadComputeShader(const std::string& _filePath)
{
	unsigned int handle{ ::rlLoadShader(_filePath.c_str(), RL_COMPUTE_SHADER) };
	ComputeShaderHandle result{ computeShaderStorage.Add(::rlLoadShaderProgramCompute(handle)) };
	::rlUnloadShader(handle);
	return result;
}

// ディスパッチ
void RaylibGPUConnecter::Dispatch(ComputeShaderHandle _shader, uint32_t _groupX, uint32_t _groupY, uint32_t _groupZ)
{
	unsigned int handle;
	if (computeShaderStorage.TryGet(_shader, handle))
	{
		::rlEnableShader(handle);
		::rlComputeShaderDispatch(_groupX, _groupY, _groupZ);
	}
}
// シェーダ破棄
void RaylibGPUConnecter::DestroyComputeShader(ComputeShaderHandle _shader)
{
	unsigned int shader;
	if (computeShaderStorage.TryGet(_shader, shader))
	{
		::rlUnloadShaderProgram(shader);
		computeShaderStorage.Remove(_shader);
	}
}

// --- 描画系シェーダ関連 ---

// 頂点シェーダ読み込み
GraphicsShaderHandle RaylibGPUConnecter::LoadVertexShader(const std::string& _filePath)
{
	return graphicsShaderStorage.Add(::LoadShader(_filePath.c_str(), nullptr));
}
// ピクセルシェーダ読み込み
GraphicsShaderHandle RaylibGPUConnecter::LoadPixelShader(const std::string& _filePath)
{
	return graphicsShaderStorage.Add(::LoadShader(nullptr, _filePath.c_str()));
}
// 頂点とピクセルシェーダ読み込み
GraphicsShaderHandle RaylibGPUConnecter::LoadGraphicsShader(const std::string& _vertexShaderFilePath, const std::string& _pixelShaderFilePath)
{
	return graphicsShaderStorage.Add(::LoadShader(_vertexShaderFilePath.c_str(), _pixelShaderFilePath.c_str()));
}
// 描画関連(頂点とピクセル)シェーダスタート
void RaylibGPUConnecter::BeginGraphicsShader(GraphicsShaderHandle _shader)
{
	Shader shader;
	if (graphicsShaderStorage.TryGet(_shader, shader))
	{
		::BeginShaderMode(shader);
	}
}
// 描画関連(頂点とピクセル)シェーダ終了
void RaylibGPUConnecter::EndGraphicsShader()
{
	::EndShaderMode();
}
// 描画関連(頂点とピクセル)シェーダ破棄
void RaylibGPUConnecter::DestroyGraphicsShader(GraphicsShaderHandle _shader)
{
	Shader shader;
	if (graphicsShaderStorage.TryGet(_shader, shader))
	{
		::UnloadShader(shader);
		graphicsShaderStorage.Remove(_shader);
	}
}

// --- バッファ関連 ---

// バッファ作成
ShaderBufferHandle RaylibGPUConnecter::CreateShaderBuffer(uint32_t _size, const void* _initialData)
{
	return bufferStorage.Add(RaylibShaderBufferResource{ ::rlLoadShaderBuffer(_size, _initialData, RL_DYNAMIC_COPY),_size });
}
// バッファ更新
void RaylibGPUConnecter::UpdateShaderBuffer(ShaderBufferHandle _buffer, const void* _data, uint32_t _size, uint32_t _offset)
{
	if (_data == nullptr || _size == 0)
	{
		return;
	}

	RaylibShaderBufferResource bufferResource;
	if (bufferStorage.TryGet(_buffer, bufferResource))
	{
		if (_offset > bufferResource.size || _size > bufferResource.size - _offset)
		{
			return;
		}

		::rlUpdateShaderBuffer(bufferResource.nativeHandle, _data, _size, _offset);
	}
}
// シェーダとバッファバインド
void RaylibGPUConnecter::BindShaderBuffer(ShaderBufferHandle _buffer, uint32_t _binding)
{
	RaylibShaderBufferResource bufferResource;
	if (bufferStorage.TryGet(_buffer, bufferResource))
	{
		::rlBindShaderBuffer(bufferResource.nativeHandle, _binding);
	}
}
// バッファ値取り出し
void RaylibGPUConnecter::ReadShaderBuffer(ShaderBufferHandle _buffer, void* _destination, uint32_t _size, uint32_t _offset)
{
	if (_destination == nullptr || _size == 0)
	{
		return;
	}

	RaylibShaderBufferResource bufferResource;
	if (bufferStorage.TryGet(_buffer, bufferResource))
	{
		if (_offset > bufferResource.size || _size > bufferResource.size - _offset)
		{
			return;
		}

		::rlReadShaderBuffer(bufferResource.nativeHandle, _destination, _size, _offset);
	}
}
// バッファ破棄
void RaylibGPUConnecter::DestroyShaderBuffer(ShaderBufferHandle _buffer)
{
	RaylibShaderBufferResource bufferResource;
	if (bufferStorage.TryGet(_buffer, bufferResource))
	{
		::rlUnloadShaderBuffer(bufferResource.nativeHandle);
		bufferStorage.Remove(_buffer);
	}
}

// 定数バッファ作成
ShaderConstantBufferHandle RaylibGPUConnecter::CreateConstantBuffer(uint32_t _size)
{
	return {};
}
// 定数バッファ更新
void RaylibGPUConnecter::UpdateConstantBuffer(ShaderConstantBufferHandle _handle, const void* _data, uint32_t _size)
{

}
// 定数バッファバインド(頂点)
void RaylibGPUConnecter::BindConstantBufferVertex(ShaderConstantBufferHandle _handle, uint32_t _slot)
{

}
// 定数バッファバインド(ピクセル)
void RaylibGPUConnecter::BindConstantBufferPixel(ShaderConstantBufferHandle _handle, uint32_t _slot)
{

}
void RaylibGPUConnecter::DestroyConstantBuffer(ShaderConstantBufferHandle _buffer)
{

}
// 定数バッファにCPUからデータを書き込むためのアドレスを取得する関数
void* RaylibGPUConnecter::GetConstantBufferAddress(ShaderConstantBufferHandle _handle)
{
	return nullptr;
}


// テクスチャをShaderに渡す。
void RaylibGPUConnecter::SetTexture(int _textureHandle, uint32_t _slot)
{
	
}

// --- 同期 ---

void RaylibGPUConnecter::ShaderBufferBarrier()
{
	// nullチェックしてnullじゃないなら呼ぶだけ
	if (memoryBarrierFunction == nullptr)
	{
		return;
	}

	memoryBarrierFunction(SHADER_STORAGE_BARRIER_BIT);
}
