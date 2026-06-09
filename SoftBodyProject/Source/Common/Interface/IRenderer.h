#pragma once

#include <string>

#include "MDMath.h"
#include "Camera.h"

class IRenderer
{
public:
	// カメラ関連
	// カメラセット
	virtual void SetCamera(const Camera& _camera) = 0;
	// ---ダブルバッファリング用---

	// 垂直同期信号を待つ
	virtual int ClearDrawScreen() = 0;
	// ダブルバッファリングの反転
	virtual int ScreenFlip() = 0;
	// ---読み込み関数---
	
	// モデルの読み込み
	virtual int LoadModel(const std::string& _fileName) = 0;
	// 画像の読み込み
	virtual int LoadGraph(const std::string& _fileName) = 0;
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
	virtual void LoadDivGraph(const std::string& _fileName, int _allNum, int _xNum, int _yNum, int _xSize, int _ySize, int* _handleBuf) = 0;
	// モデル情報セット系
	// 行列セット
	virtual void ModelSetMatrix(int _handle, const Matrix4x4& _mat) = 0;
	// ---描画関数---

	// モデル描画
	virtual void DrawModel(int _handle) = 0;
	// 画像描画
	virtual void DrawGraph(const Vector2& _pos, int _handle, bool _transFlag) = 0;
	// 球描画
	virtual void DrawSphere(const Vector3& _pos, float _radius) = 0;
	// ---リソース削除関数---

	// モデル素材削除
	virtual void DeleteModel(int _handle) = 0;
	// 画像素材削除
	virtual void DeleteGraph(int _handle) = 0;
	// デストラクタ
	virtual ~IRenderer() = default;
};