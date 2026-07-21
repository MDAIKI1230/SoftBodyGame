#include "RaylibGPUConnecter.h"

// --- シェーダ関連-- -

// コンピュートシェーダ読み込み
ComputeShaderHandle RaylibGPUConnecter::LoadComputeShader(const std::string& _shaderPath)
{

}
// シェーダとバッファバインド
void RaylibGPUConnecter::BindShaderBuffer(ShaderBufferHandle& _buffer, uint32_t _binding)
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
GraphicsShaderHandle RaylibGPUConnecter::LoadVertexShader(const std::string& _vertexShaderPath)
{

}
// ピクセルシェーダ読み込み
GraphicsShaderHandle RaylibGPUConnecter::LoadPixelShader(const std::string& _pixelShaderPath)
{

}
// シェーダとバッファバインド
void RaylibGPUConnecter::BindShaderBuffer(ShaderBufferHandle& _buffer, uint32_t _binding)
{

}
// 描画関連(頂点とピクセル)シェーダスタート
void RaylibGPUConnecter::BeginGraphicsShader(GraphicsShaderHandle& _shader)
{

}
// 描画関連(頂点とピクセル)シェーダ終了
void RaylibGPUConnecter::EndGraphicsShader()
{

}
// 描画関連(頂点とピクセル)シェーダ破棄
void RaylibGPUConnecter::DestroyGraphicsShader(GraphicsShaderHandle& _shader)
{

}

// --- バッファ関連 ---

// バッファ作成
ShaderBufferHandle RaylibGPUConnecter::CreateShaderBuffer(size_t _size, const void* _initialData)
{

}
// バッファ更新
void RaylibGPUConnecter::UpdateShaderBuffer(ShaderBufferHandle& _buffer, const void* _data, size_t _size, size_t _offset = 0)
{

}
// バッファ値取り出し
void RaylibGPUConnecter::ReadShaderBuffer(ShaderBufferHandle& _buffer, void* _destination, size_t _size, size_t _offset = 0)
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
