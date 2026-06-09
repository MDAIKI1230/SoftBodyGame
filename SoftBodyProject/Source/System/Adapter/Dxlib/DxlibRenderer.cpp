#include <DxLib.h>

#include "DxlibConvert.h"

#include "DxlibRenderer.h"

void DxlibRenderer::SetCamera(const Camera& _camera)
{
	// 位置と見る点を決める
	SetCameraPositionAndTarget_UpVecY(ToDxlib(_camera.GetPos()), ToDxlib(_camera.GetTarget()));
}

int DxlibRenderer::ClearDrawScreen()
{
	return DxLib::ClearDrawScreen();
}

int DxlibRenderer::ScreenFlip()
{
	return DxLib::ScreenFlip();
}

// モデルの読み込み
int DxlibRenderer::LoadModel(const std::string& _fileName)
{
	return DxLib::MV1LoadModel(std::wstring(_fileName.begin(), _fileName.end()).c_str());
}

// 画像の読み込み
int DxlibRenderer::LoadGraph(const std::string& _fileName)
{
	return DxLib::LoadGraph(std::wstring(_fileName.begin(), _fileName.end()).c_str());
}

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
void DxlibRenderer::LoadDivGraph(const std::string& _fileName, int _allNum, int _xNum, int _yNum, int _xSize, int _ySize, int* _handleBuf)
{
	DxLib::LoadDivGraph(std::wstring(_fileName.begin(), _fileName.end()).c_str(), _allNum, _xNum, _yNum, _xSize, _ySize, _handleBuf);
}

// モデル情報セット系
// 行列セット
void DxlibRenderer::ModelSetMatrix(int _handle, const Matrix4x4& _mat)
{
	DxLib::MV1SetMatrix(_handle, ToDxlib(_mat));
}

// ---描画関数---
// モデル描画
void DxlibRenderer::DrawModel(int _handle)
{
	DxLib::MV1DrawModel(_handle);
}

// 画像描画
void DxlibRenderer::DrawGraph(const Vector2& _pos, int _handle, bool _transFlag)
{
	DxLib::DrawGraph(_pos.x, _pos.y, _handle, _transFlag);
}

// 球描画
void DxlibRenderer::DrawSphere(const Vector3& _pos, float _radius)
{
	DxLib::DrawSphere3D(
		ToDxlib(_pos), _radius, 32,
		GetColor(255, 255, 255), GetColor(255, 255, 255),
		true);
}

// ---リソース削除関数---
// モデル素材削除
void DxlibRenderer::DeleteModel(int _handle)
{
	DxLib::DeleteGraph(_handle);
}

// 画像素材削除
void DxlibRenderer::DeleteGraph(int _handle)
{
	DxLib::DeleteGraph(_handle);
}