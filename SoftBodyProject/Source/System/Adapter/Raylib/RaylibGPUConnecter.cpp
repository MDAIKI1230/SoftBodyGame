#include "RaylibGPUConnecter.h"


// コンストラクタ
RaylibGPUConnecter::RaylibGPUConnecter()
{
	graphicsShaderStorage = std::make_unique<RaylibStorage<GraphicsShaderHandle, Shader>>();
	computeShaderStorage = std::make_unique<RaylibStorage<ComputeShaderHandle, unsigned int>>();
	bufferStorage = std::make_unique<RaylibStorage<ShaderBufferHandle, unsigned int>>();
}

// --- シェーダ関連-- -

// コンピュートシェーダ読み込み
ComputeShaderHandle RaylibGPUConnecter::LoadComputeShader(const std::string& _filePath)
{
	unsigned int handle{ ::rlLoadShader(_filePath.c_str(), RL_COMPUTE_SHADER) };
	ComputeShaderHandle result{ computeShaderStorage->Add(::rlLoadShaderProgramCompute(handle)) };
	::rlUnloadShader(handle);
	return result;
}

// ディスパッチ
void RaylibGPUConnecter::Dispatch(ComputeShaderHandle& _shader, uint32_t _groupX, uint32_t _groupY, uint32_t _groupZ)
{
	unsigned int handle;
	if (computeShaderStorage->TryGet(_shader, handle))
	{
		::rlEnableShader(handle);
		::rlComputeShaderDispatch(_groupX, _groupY, _groupZ);
	}
}
// シェーダ破棄
void RaylibGPUConnecter::DestroyComputeShader(ComputeShaderHandle& _shader)
{
	unsigned int shader;
	if (computeShaderStorage->TryGet(_shader, shader))
	{
		::rlUnloadShaderProgram(shader);
	}
}

// --- 描画系シェーダ関連 ---

// 頂点シェーダ読み込み
GraphicsShaderHandle RaylibGPUConnecter::LoadVertexShader(const std::string& _filePath)
{
	return graphicsShaderStorage->Add(::LoadShader(_filePath.c_str(), nullptr));
}
// ピクセルシェーダ読み込み
GraphicsShaderHandle RaylibGPUConnecter::LoadPixelShader(const std::string& _filePath)
{
	return graphicsShaderStorage->Add(::LoadShader(nullptr, _filePath.c_str()));
}
// 頂点とピクセルシェーダ読み込み
GraphicsShaderHandle RaylibGPUConnecter::LoadPixelShader(const std::string& _vertexShaderFilePath, const std::string& _pixelShaderFilePath)
{
	return graphicsShaderStorage->Add(::LoadShader(_vertexShaderFilePath.c_str(), _pixelShaderFilePath.c_str()));
}
// 描画関連(頂点とピクセル)シェーダスタート
void RaylibGPUConnecter::BeginGraphicsShader(GraphicsShaderHandle& _shader)
{
	Shader shader;
	if (graphicsShaderStorage->TryGet(_shader, shader))
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
void RaylibGPUConnecter::DestroyGraphicsShader(GraphicsShaderHandle& _shader)
{
	Shader shader;
	if (graphicsShaderStorage->TryGet(_shader, shader))
	{
		::UnloadShader(shader);
	}
}

// --- バッファ関連 ---

// バッファ作成
ShaderBufferHandle RaylibGPUConnecter::CreateShaderBuffer(uint32_t _size, const void* _initialData)
{
	ShaderBufferHandle result{ bufferStorage->Add(::rlLoadShaderBuffer(_size, _initialData, RL_DYNAMIC_COPY)) };
	
	return ShaderBufferHandle{ result.index,result.generation,_size };
}
// バッファ更新
void RaylibGPUConnecter::UpdateShaderBuffer(ShaderBufferHandle& _buffer, const void* _data, uint32_t _size, uint32_t _offset)
{
	if (_data == nullptr || _size == 0)
	{
		return;
	}

	if (_offset > _buffer.size || _size > _buffer.size - _offset)
	{
		return;
	}

	unsigned int handle;
	if (bufferStorage->TryGet(_buffer, handle))
	{
		::rlUpdateShaderBuffer(handle, _data, _size, _offset);
	}
}
// シェーダとバッファバインド
void RaylibGPUConnecter::BindShaderBuffer(ShaderBufferHandle& _buffer, uint32_t _binding)
{
	unsigned int handle;
	if (bufferStorage->TryGet(_buffer, handle))
	{
		::rlBindShaderBuffer(handle, _binding);
	}
}
// バッファ値取り出し
void RaylibGPUConnecter::ReadShaderBuffer(ShaderBufferHandle& _buffer, void* _destination, uint32_t _size, uint32_t _offset)
{
	if (_destination == nullptr || _size == 0)
	{
		return;
	}

	if (_offset > _buffer.size || _size > _buffer.size - _offset)
	{
		return;
	}

	unsigned int handle;
	if (bufferStorage->TryGet(_buffer, handle))
	{
		::rlReadShaderBuffer(handle, _destination, _size, _offset);
	}
}
// バッファ破棄
void RaylibGPUConnecter::DestroyShaderBuffer(ShaderBufferHandle& _buffer)
{

	unsigned int handle;
	if (bufferStorage->TryGet(_buffer, handle))
	{
		::rlUnloadShaderBuffer(handle);
	}
}

// --- 同期 ---

void RaylibGPUConnecter::ShaderBufferBarrier()
{
	
}
