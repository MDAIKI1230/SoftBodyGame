#pragma once

#include <string>

#include "MDMath.h"
#include "Color.h"
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
	// --- 設定 ---

	// ZDepth使う
	virtual int SetUseZDepth(bool _flag) = 0;
	// ZDepth書き込み
	virtual int SetWriteZDepth(bool _flag) = 0;

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
	virtual void DrawSphere(const Vector3& _pos, float _radius, const Color& _color) = 0;
	// メッシュ球描画
	virtual void DrawSphereMesh(const Vector3& _pos, float _radius, const Color& _color) = 0;
	// Box描画
	virtual void DrawBox(const Matrix4x4& _mat, const Vector3& _size, const Color& _color) = 0;
	// 線描画
	virtual void DrawLine(const Vector3& _pos1, const Vector3& _pos2, const Color& _color) = 0;

	// --- シェーダ関連 ---

	//// コンピュートシェーダ読み込み
	//virtual int LoadComputeShader(
	//	const char* shaderPath) = 0;
	//// バッファ作成
	//virtual int CreateShaderBuffer(
	//	size_t size,
	//	const void* initialData) = 0;
	//// バッファ更新
	//virtual void UpdateShaderBuffer(
	//	int buffer,
	//	const void* data,
	//	size_t size,
	//	size_t offset = 0) = 0;
	//// シェーダとバッファバインド
	//virtual void BindShaderBuffer(
	//	int buffer,
	//	uint32_t binding) = 0;
	//// ディスパッチ
	//virtual void Dispatch(
	//	int shader,
	//	uint32_t groupX,
	//	uint32_t groupY,
	//	uint32_t groupZ) = 0;
	//// バッファ値取り出し
	//virtual void ReadShaderBuffer(
	//	int buffer,
	//	void* destination,
	//	size_t size,
	//	size_t offset = 0) = 0;
	//// シェーダ破棄
	//virtual void DestroyComputeShader(
	//	int shader) = 0;
	//// バッファ破棄
	//virtual void DestroyShaderBuffer(
	//	int buffer) = 0;

	// ---リソース削除関数---

	// モデル素材削除
	virtual void DeleteModel(int _handle) = 0;
	// 画像素材削除
	virtual void DeleteGraph(int _handle) = 0;
	// デストラクタ
	virtual ~IRenderer() = default;
};
