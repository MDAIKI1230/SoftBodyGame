#include "RaylibGPUConnecter.h"


// コンストラクタ
RaylibGPUConnecter::RaylibGPUConnecter()
{
	graphicsShaderStorage = std::make_unique<GraphicsShaderStorage>();
}

// --- シェーダ関連-- -

// コンピュートシェーダ読み込み
ComputeShaderHandle RaylibGPUConnecter::LoadComputeShader(const std::string& _shaderPath)
{
	return ComputeShaderHandle{};
}
// シェーダとバッファバインド
void RaylibGPUConnecter::BindShaderBuffer(ShaderBufferHandle& _buffer, ComputeShaderHandle _binding)
{

}
// ディスパッチ
void RaylibGPUConnecter::Dispatch(ComputeShaderHandle& _shader, uint32_t _groupX, uint32_t _groupY, uint32_t _groupZ)
{

}
// シェーダ破棄
void RaylibGPUConnecter::DestroyComputeShader(ComputeShaderHandle& _shader)
{

}

// --- 描画系シェーダ関連 ---

// 頂点シェーダ読み込み
GraphicsShaderHandle RaylibGPUConnecter::LoadVertexShader(const std::string& _filePath)
{
	return graphicsShaderStorage->Add(LoadShader(_filePath.c_str(), nullptr));
}
// ピクセルシェーダ読み込み
GraphicsShaderHandle RaylibGPUConnecter::LoadPixelShader(const std::string& _filePath)
{
	return graphicsShaderStorage->Add(LoadShader(nullptr, _filePath.c_str()));
}
// 頂点とピクセルシェーダ読み込み
GraphicsShaderHandle RaylibGPUConnecter::LoadPixelShader(const std::string& _vertexShaderFilePath, const std::string& _pixelShaderFilePath)
{
	return graphicsShaderStorage->Add(LoadShader(_vertexShaderFilePath.c_str(), _pixelShaderFilePath.c_str()));
}
// シェーダとバッファバインド
void RaylibGPUConnecter::BindShaderBuffer(ShaderBufferHandle& _buffer, GraphicsShaderHandle _binding)
{

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
ShaderBufferHandle RaylibGPUConnecter::CreateShaderBuffer(size_t _size, const void* _initialData)
{
	return ShaderBufferHandle{};
}
// バッファ更新
void RaylibGPUConnecter::UpdateShaderBuffer(ShaderBufferHandle& _buffer, const void* _data, size_t _size, size_t _offset)
{

}
// バッファ値取り出し
void RaylibGPUConnecter::ReadShaderBuffer(ShaderBufferHandle& _buffer, void* _destination, size_t _size, size_t _offset)
{

}
// バッファ破棄
void RaylibGPUConnecter::DestroyShaderBuffer(ShaderBufferHandle& _buffer)
{

}

// --- 同期 ---

void RaylibGPUConnecter::ShaderBufferBarrier()
{

}
