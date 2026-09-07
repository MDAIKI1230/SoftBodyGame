#pragma once

#include <chrono>

#include "SingleTonMacros.h"

class TimeManager
{
public:
	// 更新用関数
	static void Update()
	SingletonPublicFunction(void, Update, (), ())
	// 次のフレーム始まるの待ち関数
	static void WaitNextFrame()
	SingletonPublicFunction(void, WaitNextFrame, (), ())
	// 経過時間取得
	static float GetDeltaTime()
	SingletonPublicFunction(float, GetDeltaTime, (), ())
	// 経過時間取得
	static float GetFixedDeltaTime()
	SingletonPublicFunction(float, GetFixedDeltaTime, (), ())
	// 描画と物理の補間用α値
	static float GetFixedAlpha()
	SingletonPublicFunction(float, GetFixedAlpha, (), ())
	// FixedUpdateを呼び出すタイミングか
	static bool IsFixedUpdateTime()
	SingletonPublicFunction(bool, IsFixedUpdateTime, (), ())
private:
	// シングルトン
	// コンストラクタ
	TimeManager();
	static TimeManager& Instance()
	{
		static TimeManager Instance;
		return Instance;
	}

private:
	static constexpr double FIXED_TIME{ 0.02 };

private:
	// 一フレームにかかる時間(秒)
	std::chrono::duration<double> timePerFps;
	// フレームの終了時間
	std::chrono::time_point<std::chrono::steady_clock> nextFrameTime;
	// 今の時間
	std::chrono::time_point<std::chrono::steady_clock> nowTime;
	// 前の時間
	std::chrono::time_point<std::chrono::steady_clock> pastTime;
	// 経過時間
	double deltaTime{ 0.0 };
	// 物理更新用
	double fixedTimer{ 0.0 };
};
