#include <DxLib.h>

#include "DxlibConstants.h"

#include "DxlibInput.h"

// 更新
void DxlibInput::Update()
{
	for (int i = 0; i < 256; i++)
	{
		pastKeyState[i] = currentKeyState[i];
	}
	GetHitKeyStateAll(currentKeyState);
}

// キーが押されたとき
bool DxlibInput::GetKeyPush(KeyConstants keyCode)
{
	return (currentKeyState[KeyTable[(int)keyCode]] == 1) && (pastKeyState[KeyTable[(int)keyCode]] == 0);
}

// キーが話されたとき
bool DxlibInput::GetKeyRelease(KeyConstants keyCode)
{
	return (currentKeyState[KeyTable[(int)keyCode]] == 0) && (pastKeyState[KeyTable[(int)keyCode]] == 1);
}

// キーが押されている間
bool DxlibInput::GetKeyPress(KeyConstants keyCode)
{
	return (currentKeyState[KeyTable[(int)keyCode]] == 1);
}