#pragma once

#include <chrono>

class TimeManager
{
public:
	// コンストラクタ
	TimeManager();
	// 更新用関数
	void Update();
	// 次のフレーム始まるの待ち関数
	void WaitNextFrame();
	// 経過時間取得
	float GetDeltaTime();
	// 経過時間取得
	float GetFixedDeltaTime();
	// FixedUpdateを呼び出すタイミングか
	bool IsFixedUpdateTime();
private:
	const double FIXED_TIME{ 0.04 };

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