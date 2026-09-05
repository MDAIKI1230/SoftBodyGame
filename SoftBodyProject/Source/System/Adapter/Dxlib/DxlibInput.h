#pragma once

#include <vector>

#include "IInput.h"

#include "GamePadState.h"

class DxlibInput:public IInput
{
public:
	// 更新
	void Update() override;
	// 初期化
	void Initialize() override;
	// キーボード
	bool GetCurrentValue(KeyConstants _key) const override;

	// マウス
	bool GetCurrentValue(MouseButton _button) const override;
	// ホイール
	float GetCurrentValue(MouseAxis1D _axis) const override;

	Vector2 GetCurrentValue(MouseAxis2D _axis) const override;

	// コントローラ
	bool GetCurrentValue(GamePadButton _button, std::size_t _gamePadIndex) const override;
	// -1～1(コントローラの型のボタン)
	float GetCurrentValue(GamePadAxis1D _axis, std::size_t _gamePadIndex) const override;
	// -1(左/下)～1(右/上)
	Vector2 GetCurrentValue(GamePadAxis2D _axis, std::size_t _gamePadIndex) const override;
private:
	// --- 正規化関数 ---

	float NormalizeXInputTrigger(const unsigned char _value);
	float NormalizeXInputStick(const short _value);

	float NormalizeDirectInputTrigger(const unsigned char _value);
	float NormalizeDirectInputStick(const int _value);

	// --- ゲームパッド更新関数 ---

	bool UpdateJoypadXInputState(GamePadState& _state, int _inputType);
	bool UpdateJoypadDirectInputState(GamePadState& _state, int _inputType);

	// スイッチプロコン入力読み込み
	bool UpdateSwitchProCtrl(GamePadState& _state, int _inputType);
private:
	static constexpr int MAX_GAMEPAD_COUNT{ 16 };
private:
	std::vector<GamePadState> gamePadStates{};
	int gamePadCount{ 0 };
	Vector2 mouseCurrentPos{};
	Vector2 mousePreviousPos{};
	Vector2 mouseScroll;
};
