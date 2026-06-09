#pragma once

#include "IRenderer.h"

class DxlibRenderer : public IRenderer
{
public:
	// カメラ関連
	// カメラセット
	void SetCamera(const Camera& _camera) override;
	// ---ダブルバッファリング用---
	
	// 垂直同期信号を待つ
	int ClearDrawScreen() override;
	// ダブルバッファリングの反転
	int ScreenFlip() override;
	// ---読み込み関数---
	
	// モデルの読み込み
	int LoadModel(const std::string& _fileName) override;
	// 画像の読み込み
	int LoadGraph(const std::string& _fileName) override;
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
	void LoadDivGraph(const std::string& _fileName, int _allNum, int _xNum, int _yNum, int _xSize, int _ySize, int* _handleBuf) override;
	// モデル情報セット系
	// 行列セット
	void ModelSetMatrix(int _handle, const Matrix4x4& _mat) override;
	// ---描画関数---
	
	// モデル描画
	void DrawModel(int _handle) override;
	// 画像描画
	void DrawGraph(const Vector2& _pos, int _handle, bool _transFlag) override;
	// 球描画
	void DrawSphere(const Vector3& _pos, float _radius) override;
	// ---リソース削除関数---
	
	// モデル素材削除
	void DeleteModel(int _handle) override;
	// 画像素材削除
	void DeleteGraph(int _handle) override;
};