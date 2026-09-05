#pragma once

#include "IRenderer.h"

#include "ResourceManager.h"

class DxlibRenderer : public IRenderer
{
public:
	// カメラ関連
	// カメラセット
	void SetCamera(const Matrix4x4& _view, float _near, float _far) override;
	// ---ダブルバッファリング用---
	
	// 垂直同期信号を待つ
	int ClearDrawScreen() override;
	// ダブルバッファリングの反転
	int ScreenFlip() override;
	// --- 設定 ---

	// ZDepth使う
	int SetUseZDepth(bool _flag) override;
	// ZDepth書き込み
	int SetWriteZDepth(bool _flag) override;

	// --- モデル情報セット系 ---
	
	// 行列セット
	void ModelSetMatrix(ModelHandle _handle, const Matrix4x4& _mat) override;
	// テクスチャをShaderに渡す。
	void BindTexture(TextureHandle _handle, uint32_t _slot) override;
	// キューブテクスチャをShaderに渡す。
	void BindCubeTexture(CubeTextureHandle _handle, uint32_t _slot) override;

	// ---描画関数---
	
	// モデル描画
	void DrawModel(ModelHandle _handle) override;
	// 画像描画
	void DrawTexture(TextureHandle _handle, const Vector2& _pos, bool _transFlag) override;
	// 球描画
	void DrawSphere(const Vector3& _pos, float _radius, const Color& _color) override;
	// メッシュ球描画
	void DrawSphereMesh(const Vector3& _pos, float _radius, const Color& _color) override;
	// Box描画
	void DrawBox(const Matrix4x4& _mat, const Vector3& _size, const Color& _color) override;
	// 線描画
	void DrawLine(const Vector3& _pos1, const Vector3& _pos2, const Color& _color) override;
	// カプセル描画
	void DrawCapsule(const Vector3& _pos1, const Vector3& _pos2, float _radius, const Color& _color);

private:
	// ---読み込み関数---

	// モデルの読み込み
	ModelHandle LoadModel(const std::string& _fileName) override;
	// モデル複製
	ModelHandle DuplicateModel(ModelHandle _handle) override;
	// 画像の読み込み
	TextureHandle LoadTexture(const std::string& _fileName) override;
	// CubeTextureの読み込み
	CubeTextureHandle LoadCubeTexture(const std::string& _fileName) override;
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
	void LoadDivTexture(const std::string& _fileName, int _allNum, int _xNum, int _yNum, int _xSize, int _ySize, int* _handleBuf) override;

	// ---リソース削除関数---

	// モデル素材削除
	void DeleteModel(ModelHandle _handle) override;
	// 画像素材削除
	void DeleteTexture(TextureHandle _handle) override;
	// キューブテクスチャ削除
	void DeleteCubeTexture(CubeTextureHandle _handle) override;

private:
	ResourceStorage<ModelHandle, int> modelStorage;
	ResourceStorage<TextureHandle, int> textureStorage;
	ResourceStorage<CubeTextureHandle, int> cubeTextureStorage;
};
