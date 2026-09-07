#include "ServiceLocator.h"

#include "Renderer.h"

// カメラセット
void Renderer::SetCameraImpl(const Matrix4x4& _view, float _near, float _far)
{
	ServiceLocator::GetRenderer()->SetCamera(_view, _near, _far);
}

// モデル描画
void Renderer::DrawModelImpl(ModelHandle _handle)
{
	ServiceLocator::GetRenderer()->DrawModel(_handle);
}

// モデル描画
void Renderer::DrawModelImpl(ModelHandle _handle, const Material& _material)
{
	// シェーダのハンドルの生存確認
	if (!_material.vertex.IsValid() || !_material.pixel.IsValid())
	{
		ServiceLocator::GetRenderer()->DrawModel(_handle);
		return;
	}

	auto gpuConnecter{ ServiceLocator::GetGPUConnecter() };
	auto renderer{ ServiceLocator::GetRenderer() };

	// 自作シェーダ開始
	gpuConnecter->BeginVertexShader(_material.vertex);
	gpuConnecter->BeginPixelShader(_material.pixel);

	// すべての定数バッファバインド
	for (std::size_t i{ 0 }; i < _material.cibstantBuffers.size(); i++)
	{
		gpuConnecter->BindConstantBufferVertex(_material.cibstantBuffers[i], _material.cbBindSlotNums[i]);
		gpuConnecter->BindConstantBufferPixel(_material.cibstantBuffers[i], _material.cbBindSlotNums[i]);
	}

	// すべての画像バインド
	for (std::size_t i{ 0 }; i < _material.bindTextures.size(); i++)
	{
		renderer->BindTexture(_material.bindTextures[i], _material.texBindSlotNums[i]);
	}

	// すべてのキューブテクスチャバインド
	for (std::size_t i{ 0 }; i < _material.bindCubeTextures.size(); i++)
	{
		renderer->BindCubeTexture(_material.bindCubeTextures[i], _material.cubeTexBindSlotNums[i]);
	}

	// 描画して自作シェーダモード終了
	renderer->DrawModel(_handle);
	gpuConnecter->EndGraphicsShader();
}

// 画像描画
void Renderer::DrawTextureImpl(TextureHandle _handle)
{
	ServiceLocator::GetRenderer()->DrawTexture(_handle, Vector2{}, true);
}

// 球描画
void Renderer::DrawSphereImpl(const Vector3& _pos, float _radius, const Color& _color)
{
	ServiceLocator::GetRenderer()->DrawSphere(_pos, _radius, _color);
}

// メッシュ球描画
void Renderer::DrawSphereMeshImpl(const Vector3& _pos, float _radius, const Color& _color)
{
	ServiceLocator::GetRenderer()->DrawSphereMesh(_pos, _radius, _color);
}

// Box描画
void Renderer::DrawBoxImpl(const Matrix4x4& _mat, const Vector3& _size, const Color& _color)
{
	ServiceLocator::GetRenderer()->DrawBox(_mat, _size, _color);
}

// 線描画
void Renderer::DrawLineImpl(const Vector3& _pos1, const Vector3& _pos2, const Color& _color)
{
	ServiceLocator::GetRenderer()->DrawLine(_pos1, _pos2, _color);
}

// カプセル描画
void Renderer::DrawCapsuleImpl(const Vector3& _pos1, const Vector3& _pos2, float _radius, const Color& _color)
{
	ServiceLocator::GetRenderer()->DrawCapsule(_pos1, _pos2, _radius, _color);
}
