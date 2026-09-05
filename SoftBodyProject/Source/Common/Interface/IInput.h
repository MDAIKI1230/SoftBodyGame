#pragma once

#include "MDMath.h"

#include "KeyConstants.h"
#include "MouseConstants.h"
#include "GamePadConstants.h"

class IInput
{
public:
    // 更新
	virtual void Update() = 0;
    // 初期化
    virtual void Initialize() = 0;
    // キーボード
    virtual bool GetCurrentValue(KeyConstants _key) const = 0;

    // マウス
    virtual bool GetCurrentValue(MouseButton _button) const = 0;
    // ホイール
    virtual float GetCurrentValue(MouseAxis1D _axis) const = 0;

    virtual Vector2 GetCurrentValue(MouseAxis2D _axis) const = 0;

    // コントローラ
    virtual bool GetCurrentValue(GamePadButton _button, std::size_t _gamepadIndex) const = 0;
    // -1～1(コントローラの型のボタン)
    virtual float GetCurrentValue(GamePadAxis1D _axis, std::size_t _gamepadIndex) const = 0;
    // -1(左/下)～1(右/上)
    virtual Vector2 GetCurrentValue(GamePadAxis2D _axis, std::size_t _gamepadIndex) const = 0;
	// デストラクタ
	virtual ~IInput() = default;
};
