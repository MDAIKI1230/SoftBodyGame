#pragma once

#include "MDMath.h"
#include "Color.h"

#include "SingletonMacros.h"

#include "ModelHandle.h"
#include "TextureHandle.h"
#include "CubeTextureHandle.h"
#include "Material.h"

/*
	描画に関する関数を呼べる。

*/
class Renderer
{
public:
	/// <summary>
	/// カメラセット
	/// </summary>
	/// <param name="_view">ビュー行列</param>
	/// <param name="_near">最近点</param>
	/// <param name="_far">最遠点</param>
	static void SetCamera(const Matrix4x4& _view, float _near, float _far)
	SingletonPublicFunction(void, SetCamera, (const Matrix4x4& _view, float _near, float _far), (_view, _near, _far))
	/// <summary>
	/// モデル描画
	/// </summary>
	/// <param name="_handle">モデルハンドル</param>
	static void DrawModel(ModelHandle _handle)
	SingletonPublicFunction(void , DrawModel,(ModelHandle _handle),(_handle))
	/// <summary>
	/// モデル描画
	/// </summary>
	/// <param name="_handle">モデルハンドル</param>
	/// <param name="_material">マテリアル</param>
	static void DrawModel(ModelHandle _handle, const Material& _material)
	SingletonPublicFunction(void , DrawModel,(ModelHandle _handle, const Material& _material),(_handle, _material))
	/// <summary>
	/// 画像描画
	/// </summary>
	/// <param name="_handle">画像ハンドル</param>
	static void DrawTexture(TextureHandle _handle)
	SingletonPublicFunction(void, DrawTexture, (TextureHandle _handle), (_handle))
	/// <summary>
	/// 球描画
	/// </summary>
	/// <param name="_pos">中心</param>
	/// <param name="_radius">半径</param>
	/// <param name="_color">色</param>
	static void DrawSphere(const Vector3& _pos, float _radius, const Color& _color)
	SingletonPublicFunction(void, DrawSphere, (const Vector3& _pos, float _radius, const Color& _color), (_pos, _radius, _color))
	/// <summary>
	/// メッシュ球描画
	/// </summary>
	/// <param name="_pos">中心</param>
	/// <param name="_radius">半径</param>
	/// <param name="_color">色</param>
	static void DrawSphereMesh(const Vector3& _pos, float _radius, const Color& _color)
	SingletonPublicFunction(void, DrawSphereMesh, (const Vector3& _pos, float _radius, const Color& _color), (_pos, _radius, _color))
	/// <summary>
	///  Box描画
	/// </summary>
	/// <param name="_pos">TRS行列</param>
	/// <param name="_size">ボックスの大きさ</param>
	/// <param name="_color">色</param>
	static void DrawBox(const Matrix4x4& _mat, const Vector3& _size, const Color& _color)
	SingletonPublicFunction(void, DrawBox, (const Matrix4x4& _mat, const Vector3& _size, const Color& _color), (_mat, _size, _color))
	/// <summary>
	/// 線描画
	/// </summary>
	/// <param name="_pos1">線の始点</param>
	/// <param name="_pos2">線の終点</param>
	/// <param name="_color">色</param>
	static void DrawLine(const Vector3& _pos1, const Vector3& _pos2, const Color& _color)
	SingletonPublicFunction(void, DrawLine, (const Vector3& _pos1, const Vector3& _pos2, const Color& _color), (_pos1, _pos2, _color))
	/// <summary>
	/// カプセル描画
	/// </summary>
	/// <param name="_pos1">軸の始点</param>
	/// <param name="_pos2">軸の終点</param>
	/// <param name="_color">色</param>
	static void DrawCapsule(const Vector3& _pos1, const Vector3& _pos2, float _radius, const Color& _color)
	SingletonPublicFunction(void, DrawCapsule, (const Vector3& _pos1, const Vector3& _pos2, float _radius, const Color& _color), (_pos1, _pos2, _radius, _color))
private:
	// シングルトン
	// コンストラクタ
	Renderer() = default;

	static Renderer& Instance()
	{
		static Renderer instance;

		return instance;
	}
};
