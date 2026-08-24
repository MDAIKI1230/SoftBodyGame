#include "RaylibInclude.h"

#include "RaylibRenderer.h"

#include "RaylibConvert.h"

void RaylibRenderer::SetCamera(const Camera* _camera)
{
    // 位置と見る点を決める
    raylibCamera.position = ToRaylib(_camera->GetPos());
    raylibCamera.target = ToRaylib(_camera->GetTarget());

    // 今の自作Cameraに存在しない値は固定
    raylibCamera.up = RlVector3{ 0.0f, 1.0f, 0.0f };
    raylibCamera.fovy = 60.0f;
    raylibCamera.projection = CAMERA_PERSPECTIVE;
}

int RaylibRenderer::ClearDrawScreen()
{
    BeginDrawing();
    ClearBackground(RlColor{ 0, 0, 0, 255 });

    BeginMode3D(raylibCamera);

    if (useZDepth == true)
    {
        rlEnableDepthTest();
    }
    else
    {
        rlDisableDepthTest();
    }

    if (writeZDepth == true)
    {
        rlEnableDepthMask();
    }
    else
    {
        rlDisableDepthMask();
    }

    return 0;
}

int RaylibRenderer::ScreenFlip()
{
    EndMode3D();
    EndDrawing();

    return 0;
}

// ZDepth使う
int RaylibRenderer::SetUseZDepth(bool _flag)
{
    useZDepth = _flag;

    // 描画途中で切り替えても、それ以前のバッチに影響させない
    rlDrawRenderBatchActive();

    if (_flag == true)
    {
        rlEnableDepthTest();
    }
    else
    {
        rlDisableDepthTest();
    }

    return 0;
}

// ZDepth書き込み
int RaylibRenderer::SetWriteZDepth(bool _flag)
{
    writeZDepth = _flag;

    rlDrawRenderBatchActive();

    if (_flag == true)
    {
        rlEnableDepthMask();
    }
    else
    {
        rlDisableDepthMask();
    }

    return 0;
}

// モデルの読み込み
int RaylibRenderer::LoadModel(const std::string& _fileName)
{
    if (!FileExists(_fileName.c_str()))
    {
        return -1;
    }

    Model model = ::LoadModel(_fileName.c_str());

    if (!IsModelValid(model))
    {
        return -1;
    }

    const int handle = ++nextResourceHandle;
    modelMap.emplace(handle, model);
    return handle;
}

// 画像の読み込み
int RaylibRenderer::LoadGraph(const std::string& _fileName)
{
    nextResourceHandle++;
    textureMap[nextResourceHandle] = ::LoadTexture(_fileName.c_str());
    return nextResourceHandle;
}

// CubeTextureの読み込み
int RaylibRenderer::LoadCubeTexture(const std::string& _fileName)
{
    return -1;
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
void RaylibRenderer::LoadDivGraph(const std::string& _fileName, int _allNum, int _xNum, int _yNum, int _xSize, int _ySize, int* _handleBuf)
{
    
}

// モデル情報セット系
// 行列セット
void RaylibRenderer::ModelSetMatrix(int _handle, const Matrix4x4& _mat)
{
    modelMap[_handle].transform = ToRaylib(_mat);
}

// ---描画関数---
// モデル描画
void RaylibRenderer::DrawModel(int _handle)
{
    ::DrawModel(modelMap[_handle], RlVector3{ 0.0f, 0.0f, 0.0f }, 1.0f, RlColor{ 255, 255, 255, 255 });
}

// 画像描画
void RaylibRenderer::DrawGraph(const Vector2& _pos, int _handle, bool _transFlag)
{
    ::DrawTexture(textureMap[_handle], static_cast<int>(_pos.x), static_cast<int>(_pos.y), RlColor{ 255, 255, 255, 255 });
}

// 球描画
void RaylibRenderer::DrawSphere(const Vector3& _pos, float _radius, const Color& _color)
{
    ::DrawSphere(ToRaylib(_pos), _radius, ToRaylib(_color));
}

// メッシュ球描画
void RaylibRenderer::DrawSphereMesh(const Vector3& _pos, float _radius, const Color& _color)
{
    ::DrawSphereWires(ToRaylib(_pos), _radius, 8, 8, ToRaylib(_color));
}

// Box描画
void RaylibRenderer::DrawBox(const Matrix4x4& _mat, const Vector3& _size, const Color& _color)
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
            ;; DrawLine3D(ToRaylib(point[a]), ToRaylib(point[b]), ToRaylib(_color));
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
void RaylibRenderer::DrawLine(const Vector3& _pos1, const Vector3& _pos2, const Color& _color)
{
    ::DrawLine3D(ToRaylib(_pos1), ToRaylib(_pos2), ToRaylib(_color));
}
// カプセル描画
void RaylibRenderer::DrawCapsule(const Vector3& _pos1, const Vector3& _pos2, float _radius, const Color& _color)
{
    ::DrawCapsuleWires(ToRaylib(_pos1), ToRaylib(_pos2), _radius, 8, 8, ToRaylib(_color));
}
// ---リソース削除関数---
// モデル素材削除
void RaylibRenderer::DeleteModel(int _handle)
{
    ::UnloadModel(modelMap[_handle]);
}

// 画像素材削除
void RaylibRenderer::DeleteGraph(int _handle)
{
    ::UnloadTexture(textureMap[_handle]);
}
