#pragma once

class EditorGui
{
public:
    // Engine初期化後に一回呼ぶ
    bool Initialize();
    // フレーム開始時に一回呼ぶ
    void BeginFrame();
    // 描画
    void Render();
    // 終了処理(Engine終了前に呼ぶ)
    void Shutdown();
};