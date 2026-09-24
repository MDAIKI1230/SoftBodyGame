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
void Renderer::DrawTextureImpl(TextureHandle _handle, const Vector3& _position)
{
	ServiceLocator::GetRenderer()->DrawTexture(_handle, Vector2{ _position.x,_position.y }, true);
}

// Fade描画
void Renderer::DrawFadeImpl(float _alpha)
{
	ServiceLocator::GetRenderer()->DrawFadeBox(_alpha);
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

// リング描画
void Renderer::DrawRingImpl(const Vector3& _pos, const Vector3& _normal, float _radius, int _division, const Color& _color)
{
	if (_radius <= 0.0f || _division < 3)
	{
		return;
	}

	if (_normal.LengthSqr() <= MathConstants::EPSILON * MathConstants::EPSILON)
	{
		return;
	}

	Vector3 normal{ _normal.Normalized() };

	// normalと平行になりにくい基準軸を選ぶ
	float rightDot{ Vector3::Dot(normal, Vector3::RIGHT) };

	Vector3 seed{ rightDot * rightDot < 0.81f ? Vector3::RIGHT : Vector3::UP };

	// リング面を張る直交軸
	Vector3 tangent{ Vector3::Cross(normal, seed).Normalized() };

	Vector3 bitangent{ Vector3::Cross(normal, tangent) };

	float angleStep{ MathConstants::PI_FLT * 2.0f / static_cast<float>(_division) };

	Vector3 previousPoint{ _pos + tangent * _radius };

	auto renderer{ ServiceLocator::GetRenderer() };

	for (int i{ 1 }; i <= _division; i++)
	{
		float angle{ angleStep * static_cast<float>(i) };

		Vector3 point{ _pos + tangent * (std::cos(angle) * _radius) + bitangent * (std::sin(angle) * _radius) };

		renderer->DrawLine(previousPoint, point, _color);

		previousPoint = point;
	}
}

// 円錐描画
void Renderer::DrawConeImpl(const Vector3& _topPos, const Vector3& _bottomPos, float _radius, int _division, const Color& _color)
{
	ServiceLocator::GetRenderer()->DrawCone(_topPos, _bottomPos, _radius, _division, _color);
}

// 扇形描画
void Renderer::DrawSectorImpl(
	const Vector3& _pos, const Vector3& _normal, const Vector3& _reference,
	float _radius, float _startAngle, float _endAngle, int _division, const Color& _color)
{
	if (_radius <= 0.0f || _division < 1)
	{
		return;
	}

	if (_normal.LengthSqr() <= MathConstants::EPSILON * MathConstants::EPSILON)
	{
		return;
	}

	Vector3 normal{ _normal.Normalized() };

	// 基準方向を扇形の平面上へ投影
	Vector3 reference{ _reference - normal * Vector3::Dot(_reference, normal) };

	if (reference.LengthSqr() <= MathConstants::EPSILON * MathConstants::EPSILON)
	{
		return;
	}

	reference.Normalize();

	// 正の角度方向
	Vector3 tangent{ Vector3::Cross(normal, reference).Normalized() };

	float angleRange{ _endAngle - _startAngle };

	if (std::abs(angleRange) <= MathConstants::EPSILON)
	{
		return;
	}

	float angleStep{ angleRange / static_cast<float>(_division) };

	Vector3 startPoint{ _pos + (reference * std::cos(_startAngle) + tangent * std::sin(_startAngle)) * _radius };

	Vector3 previousPoint{ startPoint };

	for (int i{ 1 }; i <= _division; i++)
	{
		float angle{ _startAngle + angleStep * static_cast<float>(i) };

		Vector3 point{ _pos + (reference * std::cos(angle) + tangent * std::sin(angle)) * _radius };

		ServiceLocator::GetRenderer()->DrawLine(previousPoint, point, _color);

		previousPoint = point;
	}

	// 扇形の両端
	ServiceLocator::GetRenderer()->DrawLine(_pos, startPoint, _color);
	ServiceLocator::GetRenderer()->DrawLine(_pos, previousPoint, _color);
}