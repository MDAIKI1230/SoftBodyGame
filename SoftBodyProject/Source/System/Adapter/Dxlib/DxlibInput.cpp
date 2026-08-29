#include <DxLib.h>

#include "DxlibConstants.h"

#include "DxlibInput.h"

// 更新
void DxlibInput::Update()
{
	gamePadCount = DxLib::GetJoypadNum();

	if (gamePadCount < 0)
	{
		gamePadCount = 0;
	}
	else if (gamePadCount > MAX_GAMEPAD_COUNT)
	{
		gamePadCount = MAX_GAMEPAD_COUNT;
	}

	gamePadStates.resize(gamePadCount);

	for (int i{ 0 }; i < gamePadCount; i++)
	{
		GamePadState& gamePadState{ gamePadStates[i] };

		// 切断時や取得失敗時に古い値を残さない
		gamePadState = {};

		// 状態の取得
		DxLib::XINPUT_STATE state{};

		int inputType{ DX_INPUT_PAD1 + i };

		if (DxLib::GetJoypadXInputState(inputType, &state) == -1)
		{
			continue;
		}

		gamePadState.connected = true;

		// ボタン
		for (std::size_t buttonIndex{ 0 }; buttonIndex < (size_t)GamePadButton::Count; buttonIndex++)
		{
			// ボタンの列挙体すべての入力値を写す
			int nativeButtonIndex{ DxlibConstants::DxlibXInputButtonTable[buttonIndex] };

			if (nativeButtonIndex == DxlibConstants::DXLIB_INVALID_INPUT)
			{
				continue;
			}

			gamePadState.buttons[buttonIndex] = state.Buttons[nativeButtonIndex];
		}

		// トリガー
		for (std::size_t axisIndex{ 0 }; axisIndex < (size_t)GamePadAxis1D::Count; axisIndex++)
		{
			auto member{ DxlibConstants::DxlibXInputAxis1DTable[axisIndex] };

			gamePadState.axis1D[axisIndex] = NormalizeXInputTrigger(state.*member);
		}

		// スティック
		for (std::size_t axisIndex{ 0 }; axisIndex < (size_t)GamePadAxis2D::Count; axisIndex++)
		{
			auto& members{ DxlibConstants::DxlibXInputAxis2DTable[axisIndex] };

			gamePadState.axis2D[axisIndex] = Vector2{
				NormalizeXInputStick(state.*members.x),
				NormalizeXInputStick(state.*members.y)
			};
		}
	}

	// マウスポインタの位置情報更新
	int x, y;
	DxLib::GetMousePoint(&x, &y);

	mousePreviousPos = mouseCurrentPos;
	mouseCurrentPos = Vector2{ static_cast<float>(x),static_cast<float>(y) };

	mouseScroll = Vector2{
	DxLib::GetMouseHWheelRotVolF(),
	DxLib::GetMouseWheelRotVolF()
	};
}

// 初期化
void DxlibInput::Initialize()
{
	// マウスポインタの位置情報更新
	int x, y;
	DxLib::GetMousePoint(&x, &y);

	mousePreviousPos = Vector2{ static_cast<float>(x),static_cast<float>(y) };
	mouseCurrentPos = Vector2{ static_cast<float>(x),static_cast<float>(y) };
}

// キーボード
bool DxlibInput::GetCurrentValue(KeyConstants _key) const
{
	return DxLib::CheckHitKey(DxlibConstants::KeyTable[(int)_key]);
}

// マウス
bool DxlibInput::GetCurrentValue(MouseButton _button) const
{
    return (GetMouseInput() & DxlibConstants::DxlibMouseButtonTable[(int)_button]);
}

float DxlibInput::GetCurrentValue(MouseAxis1D _axis) const
{
	switch (_axis)
	{
	case MouseAxis1D::SCROLL_X:
		return mouseScroll.x;
		break;
	case MouseAxis1D::SCROLL_Y:
		return mouseScroll.y;
		break;
	default:
		return 0.0f;
		break;
	}
}

Vector2 DxlibInput::GetCurrentValue(MouseAxis2D _axis) const
{
	switch (_axis)
	{
	case MouseAxis2D::POSITION:
		return mouseCurrentPos;
		break;
	case MouseAxis2D::DELTA:
		return mouseCurrentPos - mousePreviousPos;
		break;
	case MouseAxis2D::SCROLL:
		return mouseScroll;
		break;
	default:
		return {};
		break;
	}
}

// コントローラ
bool DxlibInput::GetCurrentValue(GamePadButton _button, std::size_t _gamePadIndex) const
{
	// コントローラ番号のチェック
	if (_gamePadIndex >= gamePadCount)
	{
		return false;
	}

	return gamePadStates[_gamePadIndex].buttons[(int)_button];
}

// -1～1(コントローラの型のボタン)
float DxlibInput::GetCurrentValue(GamePadAxis1D _axis, std::size_t _gamePadIndex) const
{
	// コントローラ番号のチェック
	if (_gamePadIndex >= gamePadCount)
	{
		return 0.0f;
	}

	return gamePadStates[_gamePadIndex].axis1D[(int)_axis];
}

// -1(左/下)～1(右/上)
Vector2 DxlibInput::GetCurrentValue(GamePadAxis2D _axis, std::size_t _gamePadIndex) const
{
	// コントローラ番号のチェック
	if (_gamePadIndex >= gamePadCount)
	{
		return {};
	}

	return gamePadStates[_gamePadIndex].axis2D[(int)_axis];
}

// --- 正規化関数 ---

float DxlibInput::NormalizeXInputTrigger(const unsigned char _value)
{
	return static_cast<float>(_value) / 255.0f;
}

float DxlibInput::NormalizeXInputStick(const short _value)
{
	if (_value < 0)
	{
		return static_cast<float>(_value) / 32768.0f;
	}

	return static_cast<float>(_value) / 32767.0f;
}
