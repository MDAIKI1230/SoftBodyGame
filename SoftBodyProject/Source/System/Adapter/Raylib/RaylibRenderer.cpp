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

