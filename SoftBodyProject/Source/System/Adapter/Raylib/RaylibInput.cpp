#include "RaylibInclude.h"

#include "RaylibConstants.h"

#include "RaylibInput.h"

// 更新
void RaylibInput::Update()
{
	// Raylibは何もいらないらしい。
}

// キーが押されたとき
bool RaylibInput::GetKeyPush(KeyConstants keyCode)
{
	return ::IsKeyPressed(KeyTable[(int)keyCode]);
}

// キーが話されたとき
bool RaylibInput::GetKeyRelease(KeyConstants keyCode)
{
	return ::IsKeyReleased(KeyTable[(int)keyCode]);
}

// キーが押されている間
bool RaylibInput::GetKeyPress(KeyConstants keyCode)
{
	return IsKeyDown(KeyTable[(int)keyCode]);
}
