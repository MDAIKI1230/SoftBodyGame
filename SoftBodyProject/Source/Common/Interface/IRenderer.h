#pragma once

#include <string>

#include "MDMath.h"
#include "Color.h"
#include "SkeletonData.h"
#include "Buffer/PoseBuffer.h"

#include "ModelHandle.h"
#include "TextureHandle.h"
#include "CubeTextureHandle.h"

class IRenderer
{
	friend class ResourceManager;
	friend class Renderer;
	friend class ApplicationManager;
public:
	// コンストラクタ
	IRenderer() = default;
	// デストラクタ
	virtual ~IRenderer() = default;

protected:
	// カメラ関連
	// カメラセット
	virtual void SetCamera(const Matrix4x4& _view, float _near, float _far) = 0;
	// ---ダブルバッファリング用---

	// 垂直同期信号を待つ
	virtual int ClearDrawScreen() = 0;
	// ダブルバッファリングの反転
	virtual int ScreenFlip() = 0;
	// --- 設定 ---

	// ZDepth使う
	virtual int SetUseZDepth(bool _flag) = 0;
	// ZDepth書き込み
	virtual int SetWriteZDepth(bool _flag) = 0;

	// --- モデル情報セット系 ---
	
	// 行列セット
	virtual void ModelSetMatrix(ModelHandle _handle, const Matrix4x4& _mat) = 0;
	// テクスチャをShaderに渡す。
	virtual void BindTexture(TextureHandle _handle, uint32_t _slot) = 0;
	// キューブテクスチャをShaderに渡す。
	virtual void BindCubeTexture(CubeTextureHandle _handle, uint32_t _slot) = 0;

	// ---描画関数---

	// モデル描画
	virtual void DrawModel(ModelHandle _handle) = 0;
	// 画像描画
	virtual void DrawTexture(TextureHandle _handle, const Vector2& _pos, bool _transFlag) = 0;
	// 球描画
	virtual void DrawSphere(const Vector3& _pos, float _radius, const Color& _color) = 0;
	// メッシュ球描画
	virtual void DrawSphereMesh(const Vector3& _pos, float _radius, const Color& _color) = 0;
	// Box描画
	virtual void DrawBox(const Matrix4x4& _mat, const Vector3& _size, const Color& _color) = 0;
	// 線描画
	virtual void DrawLine(const Vector3& _pos1, const Vector3& _pos2, const Color& _color) = 0;
	// カプセル描画
	virtual void DrawCapsule(const Vector3& _pos1, const Vector3& _pos2, float _radius, const Color& _color) = 0;

	// --- アニメーション関連 ---

	// モデルのスケルトンデータの取得
	virtual bool GetSkeletonData(ModelHandle _handle, SkeletonData& _output) = 0;
	// 現在のスケルトンのポーズ情報の取得
	virtual bool GetCurrentPose(ModelHandle _handle, PoseBuffer& _output) = 0;
	// モデルに、ポーズを適応する
	virtual bool ApplyPose(ModelHandle _handle, const PoseBuffer& _pose) = 0;

	// ---読み込み関数---

	// モデルの読み込み
	virtual ModelHandle LoadModel(const std::string& _fileName) = 0;
	// モデル複製
	virtual ModelHandle DuplicateModel(ModelHandle _handle) = 0;
	// 画像の読み込み
	virtual TextureHandle LoadTexture(const std::string& _fileName) = 0;
	// CubeTextureの読み込み
	virtual CubeTextureHandle LoadCubeTexture(const std::string& _fileName) = 0;
	/// <summary>
	/// 画像の分割読み込み
	/// </summary>
	/// <param name="_fileName">ファイル名</param>
	/// <param name="_allNum">分割全体数</param>
	/// <param name="_xNum">横の数</param>
	/// <param name="_yNum">縦の数</param>
	/// <param name="_xSize">分割した一つの横幅</param>
	/// <param name="_ySize">分割した一つの縦幅</param>
	/// <param name="handleBuf">配列のアドレス</param>
	virtual void LoadDivTexture(const std::string& _fileName, int _allNum, int _xNum, int _yNum, int _xSize, int _ySize, int* _handleBuf) = 0;

	// ---リソース削除関数---

	// モデル素材削除
	virtual void DeleteModel(ModelHandle _handle) = 0;
	// 画像素材削除
	virtual void DeleteTexture(TextureHandle _handle) = 0;
	// キューブテクスチャ削除
	virtual void DeleteCubeTexture(CubeTextureHandle _handle) = 0;

	// すべてのリソースを削除
	virtual void DeleteAll() = 0;
};
