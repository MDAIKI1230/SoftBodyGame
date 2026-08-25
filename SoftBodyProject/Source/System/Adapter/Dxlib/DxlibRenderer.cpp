#include <DxLib.h>

#include "DxlibConvert.h"

#include "DxlibRenderer.h"

void DxlibRenderer::SetCamera(const Camera* _camera)
{
	// 位置と見る点を決める
    DxLib::SetCameraPositionAndTargetAndUpVec(ToDxlib(_camera->GetPos()), ToDxlib(_camera->GetTarget()), ToDxlib(Vector3::UP));
    DxLib::SetCameraNearFar(0.1f, 1000.0f);
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
ModelHandle DxlibRenderer::LoadModel(const std::string& _fileName)
{
    return modelStorage.Add(DxLib::MV1LoadModel(std::wstring(_fileName.begin(), _fileName.end()).c_str()));
}

// モデル複製
ModelHandle DxlibRenderer::DuplicateModel(ModelHandle _handle)
{
    int nativeHandle;

    if (modelStorage.TryGet(_handle, nativeHandle))
    {
        return modelStorage.Add(DxLib::MV1DuplicateModel(nativeHandle));
    }
}

// 画像の読み込み
TextureHandle DxlibRenderer::LoadTexture(const std::string& _fileName)
{
    return textureStorage.Add(DxLib::LoadGraph(std::wstring(_fileName.begin(), _fileName.end()).c_str()));
}

// CubeTextureの読み込み
CubeTextureHandle DxlibRenderer::LoadCubeTexture(const std::string& _fileName)
{
    const std::wstring filePath(_fileName.begin(),_fileName.end());

    DxLib::SetCubeMapTextureCreateFlag(TRUE);

    int handle{ DxLib::LoadGraph(std::wstring(_fileName.begin(), _fileName.end()).c_str()) };

    // 元に戻す
    DxLib::SetCubeMapTextureCreateFlag(FALSE);

    return cubeTextureStorage.Add(handle);
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
void DxlibRenderer::LoadDivTexture(const std::string& _fileName, int _allNum, int _xNum, int _yNum, int _xSize, int _ySize, int* _handleBuf)
{
	DxLib::LoadDivGraph(std::wstring(_fileName.begin(), _fileName.end()).c_str(), _allNum, _xNum, _yNum, _xSize, _ySize, _handleBuf);
}

// モデル情報セット系
// 行列セット
void DxlibRenderer::ModelSetMatrix(ModelHandle _handle, const Matrix4x4& _mat)
{
    int nativeHandle;

    if (modelStorage.TryGet(_handle, nativeHandle))
    {
        DxLib::MV1SetMatrix(nativeHandle, ToDxlib(_mat));
    }
}

// ---描画関数---
// モデル描画
void DxlibRenderer::DrawModel(ModelHandle _handle)
{
    int nativeHandle;

    if (modelStorage.TryGet(_handle, nativeHandle))
    {
        DxLib::MV1DrawModel(nativeHandle);
    }
}

// 画像描画
void DxlibRenderer::DrawTexture(TextureHandle _handle, const Vector2& _pos, bool _transFlag)
{
    int nativeHandle;

    if (textureStorage.TryGet(_handle, nativeHandle))
    {
        DxLib::DrawGraph(static_cast<int>(_pos.x), static_cast<int>(_pos.y), nativeHandle, _transFlag);
    }
}

// 球描画
void DxlibRenderer::DrawSphere(const Vector3& _pos, float _radius, const Color& _color)
{
	DxLib::DrawSphere3D(
		ToDxlib(_pos), _radius, 8,
		ToDxlib(_color), ToDxlib(_color),
		true);
}

// メッシュ球描画
void DxlibRenderer::DrawSphereMesh(const Vector3& _pos, float _radius, const Color& _color)
{
    DxLib::DrawSphere3D(
        ToDxlib(_pos), _radius, 8,
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
// 線描画
void DxlibRenderer::DrawLine(const Vector3& _pos1, const Vector3& _pos2, const Color& _color)
{
    DxLib::DrawLine3D(ToDxlib(_pos1), ToDxlib(_pos2), ToDxlib(_color));
}
// カプセル描画
void DxlibRenderer::DrawCapsule(const Vector3& _pos1, const Vector3& _pos2, float _radius, const Color& _color)
{
    DxLib::DrawCapsule3D(ToDxlib(_pos1), ToDxlib(_pos2), _radius, 10, ToDxlib(_color), ToDxlib(_color), false);
}

// テクスチャをShaderに渡す。
void DxlibRenderer::BindTexture(TextureHandle _handle, uint32_t _slot)
{
    int nativeHandle;

    if (textureStorage.TryGet(_handle, nativeHandle))
    {
        DxLib::SetUseTextureToShader(_slot, nativeHandle);
    }
}
// キューブテクスチャをShaderに渡す。
void DxlibRenderer::BindCubeTexture(CubeTextureHandle _handle, uint32_t _slot)
{
    int nativeHandle;

    if (cubeTextureStorage.TryGet(_handle, nativeHandle))
    {
        DxLib::SetUseTextureToShader(_slot, nativeHandle);
    }
}

// モデル素材削除
void DxlibRenderer::DeleteModel(ModelHandle _handle)
{
    int nativeHandle;

    if (modelStorage.TryGet(_handle, nativeHandle))
    {
        DxLib::MV1DeleteModel(nativeHandle);

        modelStorage.Remove(_handle);
    }
}

// 画像素材削除
void DxlibRenderer::DeleteTexture(TextureHandle _handle)
{
    int nativeHandle;

    if (textureStorage.TryGet(_handle, nativeHandle))
    {
        DxLib::DeleteGraph(nativeHandle);

        textureStorage.Remove(_handle);
    }
}

// キューブテクスチャ削除
void DxlibRenderer::DeleteCubeTexture(CubeTextureHandle _handle)
{
    int nativeHandle;

    if (cubeTextureStorage.TryGet(_handle, nativeHandle))
    {
        DxLib::DeleteGraph(nativeHandle);

        cubeTextureStorage.Remove(_handle);
    }
}
