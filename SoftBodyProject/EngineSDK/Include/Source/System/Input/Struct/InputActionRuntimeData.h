#pragma once

#include <vector>

#include "InputValue.h"
#include "../InputActionContext.h"

enum class InputActionPhase
{
    DISABLED,   // ActionMapが無効
    WAITING,    // 入力待ち
    STARTED,    // 入力が始まった
    PERFORMED,  // 発動条件を満たした
    CANCELED    // 入力が終了・中断された
};

// ランタイム中の入力状態構造体
struct InputActionRuntimeState
{
    InputValue previous;
    InputValue current;
    InputActionPhase phase{ InputActionPhase::WAITING };
    float interactionTime{ 0.0f };
};

struct InputCallbackEntry
{
	using InputActionCallbackFunc = void(*)(void*, InputActionContext&);

    InputActionCallbackFunc function{ nullptr };

    void* userData{ nullptr };

    // コールバック実行中に解除されたとき用
    bool active{ true };
};

struct InputActionCallbacks
{
    std::vector<InputCallbackEntry> started;
    std::vector<InputCallbackEntry> performed;
    std::vector<InputCallbackEntry> canceled;
};

struct InputActionRuntimeData
{
    // 入力値・Phase・時間
    InputActionRuntimeState state;

    // 登録された関数
    InputActionCallbacks callbacks;
};
