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
	static void DrawTexture(TextureHandle _handle, const Vector3& _position)
	SingletonPublicFunction(void, DrawTexture, (TextureHandle _handle, const Vector3& _position), (_handle, _position))
	/// <summary>
	/// Fade描画
	/// </summary>
	static void DrawFade(float _alpha)
	SingletonPublicFunction(void, DrawFade, (float _alpha), (_alpha))
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
	/// <summary>
	/// リング描画
	/// </summary>
	/// <param name="_pos">中心</param>
	/// <param name="_normal">リング面の法線</param>
	/// <param name="_radius">半径</param>
	/// <param name="_division">分割数</param>
	/// <param name="_color">色</param>
	static void DrawRing(const Vector3& _pos, const Vector3& _normal, float _radius, int _division, const Color& _color)
	SingletonPublicFunction(void, DrawRing, (const Vector3& _pos, const Vector3& _normal, float _radius, int _division, const Color& _color), (_pos, _normal, _radius, _division, _color))
	/// <summary>
	/// 円錐描画
	/// </summary>
	/// <param name="_topPos">頂点</param>
	/// <param name="_bottomPos">底面の中心</param>
	/// <param name="_radius">底面の半径</param>
	/// <param name="_division">分割数</param>
	/// <param name="_color">色</param>
	static void DrawCone(const Vector3& _topPos, const Vector3& _bottomPos, float _radius, int _division, const Color& _color)
	SingletonPublicFunction(void, DrawCone, (const Vector3& _topPos, const Vector3& _bottomPos, float _radius, int _division, const Color& _color), (_topPos, _bottomPos, _radius, _division, _color))
	/// <summary>
	/// 扇形描画
	/// </summary>
	/// <param name="_pos">中心</param>
	/// <param name="_normal">扇形面の法線</param>
	/// <param name="_reference">角度0の基準方向</param>
	/// <param name="_radius">半径</param>
	/// <param name="_startAngle">開始角度</param>
	/// <param name="_endAngle">終了角度</param>
	/// <param name="_division">円弧の分割数</param>
	/// <param name="_color">色</param>
	static void DrawSector(
		const Vector3& _pos, const Vector3& _normal, const Vector3& _reference,
		float _radius, float _startAngle, float _endAngle, int _division, const Color& _color)
	SingletonPublicFunction(void, DrawSector,
		(const Vector3& _pos, const Vector3& _normal, const Vector3& _reference,
			float _radius, float _startAngle, float _endAngle, int _division, const Color& _color),
		(_pos, _normal, _reference, _radius, _startAngle, _endAngle, _division, _color))
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
