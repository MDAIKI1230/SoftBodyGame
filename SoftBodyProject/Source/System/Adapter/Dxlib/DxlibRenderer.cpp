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

// ZDepth使う
int DxlibRenderer::SetUseZDepth(bool _flag)
{
	return DxLib::SetUseZBuffer3D(_flag);
}

// ZDepth書き込み
int DxlibRenderer::SetWriteZDepth(bool _flag)
{
	return DxLib::SetWriteZBuffer3D(_flag);
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
	MV1SetUseZBuffer(_handle, true);
	MV1SetWriteZBuffer(_handle, true);

	DxLib::MV1DrawModel(_handle);
}

// 画像描画
void DxlibRenderer::DrawGraph(const Vector2& _pos, int _handle, bool _transFlag)
{
	DxLib::DrawGraph(_pos.x, _pos.y, _handle, _transFlag);
}

// 球描画
void DxlibRenderer::DrawSphere(const Vector3& _pos, float _radius, const Color& _color)
{
	DxLib::DrawSphere3D(
		ToDxlib(_pos), _radius, 32,
		ToDxlib(_color), ToDxlib(_color),
		true);
}

// メッシュ球描画
void DxlibRenderer::DrawSphereMesh(const Vector3& _pos, float _radius, const Color& _color)
{
    DxLib::DrawSphere3D(
        ToDxlib(_pos), _radius, 32,
        ToDxlib(_color), ToDxlib(_color),
        false);
}

// Box描画
void DxlibRenderer::DrawBox(const Matrix4x4& _mat, const Vector3& _size, const Color& _color)
{
    Vector3 halfSize = _size * 0.5f;

    // ローカル8頂点
    Vector3 point[8] = {
        {-halfSize.x, -halfSize.y, -halfSize.z},
        { halfSize.x, -halfSize.y, -halfSize.z},
        { halfSize.x,  halfSize.y, -halfSize.z},
        {-halfSize.x,  halfSize.y, -halfSize.z},

        {-halfSize.x, -halfSize.y,  halfSize.z},
        { halfSize.x, -halfSize.y,  halfSize.z},
        { halfSize.x,  halfSize.y,  halfSize.z},
        {-halfSize.x,  halfSize.y,  halfSize.z},
    };

    // ワールド変換
    for (int i = 0; i < 8; i++)
    {
        point[i] = _mat * point[i];
    }

    // エッジ12本
    auto L = [&](int a, int b)
        {
            DrawLine3D(ToDxlib(point[a]), ToDxlib(point[b]), ToDxlib(_color));
        };

    // 下
    L(0, 1); L(1, 2); L(2, 3); L(3, 0);

    // 上
    L(4, 5); L(5, 6); L(6, 7); L(7, 4);

    // 横の線
    L(0, 4);
    L(1, 5);
    L(2, 6);
    L(3, 7);
}

void DxlibRenderer::DrawLine(const Vector3& _pos1, const Vector3& _pos2, const Color& _color)
{
    DxLib::DrawLine3D(ToDxlib(_pos1), ToDxlib(_pos2), ToDxlib(_color));
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
